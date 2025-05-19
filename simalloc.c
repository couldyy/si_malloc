#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <signal.h>
#include "simalloc_internal.h" 
#include "include/simalloc.h" 
#include "simalloc_utils.h" 



Arena* memory_arena = NULL;

//TODO: Now the memory is completly unprotected, so the user can do just arr[-1] and get to 
// si_malloc metadata or even worse, do some malicious things
// provide some protection if it is possible
void* si_malloc(size_t n) 
{
    if(n <= 0) {
        fprintf(stderr, "si_malloc(): invalid size\n");
        return NULL;
    }
    if (memory_arena == NULL) {
        memory_arena = init_ci_malloc();
    }

    // allocate memory for servica data + user required memory
    size_t metadata_size = sizeof(struct MemoryChunk);
    int total_requested_size = ALIGN(n) + metadata_size;   // alligned user requested bytes + size of metadata
    void* ptr = sbrk(total_requested_size);
#ifdef DEBUG_MALLOC
    void* prev_ptr = sbrk(0);
    printf("malloc'ed size: %ld, requested: %zu\n", prev_ptr - ptr, n);
#endif
    //void* ptr = sbrk(0);
    struct MemoryChunk* prev_chunk = NULL;
    struct MemoryChunk* next_chunk = NULL;
    struct MemoryChunk new_chunk = {total_requested_size - metadata_size, next_chunk, prev_chunk};
    
    *(struct MemoryChunk*)ptr = new_chunk;
    char* ret_ptr = (char*)ptr + metadata_size;
    return (void*)ret_ptr; 
}


void si_free(void* ptr)
{
    //struct MemoryChunk* this_chunk = (struct MemoryChunk*)((char*)ptr - sizeof(struct MemoryChunk));
    struct MemoryChunk* this_chunk = GET_METADATA_PTR(ptr);
    int user_data_size = this_chunk->size;


    // FIXME: Fix issue when we have already freed some ptr
    // and then some other function using sbrk() moved head after current address making possible no double free detection
    // Example:
    //
    //      int *var1 = si_malloc(sizeof(int));     <-- alloc memory using sbrk()
    //      *var1 = 10;
    //      si_free(var1);                          <-- free memory using sbrk(-size), now var1 > sbrk(0)
    //      sbrk(sizeof(int) * 20);                 <-- simulation of some function allocating memory using sbrk(), now var1 < sbrk() but we didnt allocate any memory
    //      si_free(var1);                          <-- free memory of other function, NO DOUBLE FREE DETECTION
    //
    // check if memory was freed before using sbrk(-size)
    if(sbrk(0) < (ptr + this_chunk->size)){
        fprintf(stderr, "si_free(): double free detected\n");
        kill(getpid(), SIGABRT);
    }
    // return memory to system if it is possible
    // (if sbrk() points to the end of a chunk)
    else if(sbrk(0) == (ptr + this_chunk->size)){
        memset(ptr, 0, this_chunk->size);     // zero out memory
        sbrk(-(this_chunk->size + sizeof(struct MemoryChunk)));
#ifdef DEBUG_FREE
        //printf("returned %d bytes to system\n", user_data_size);
#endif
        this_chunk = NULL;
        return;
    }


    // look for that mem address in free chunks
    // if found -  abort
    struct MemoryChunk *temp_free_chunk = memory_arena->free_chunks;
    while(temp_free_chunk != NULL) {
        if(((char*)temp_free_chunk + sizeof(struct MemoryChunk)) == ptr) {
            fprintf(stderr, "si_free(): double free detected\n");
            kill(getpid(), SIGABRT);
            break;
        }
        temp_free_chunk = temp_free_chunk->next_chunk;
    }
    
    memset(ptr, 0, user_data_size);     // zero out memory

    //temp_free_chunk = memory_arena->free_chunks;
    
    append_free_chunks_list(memory_arena->free_chunks, this_chunk);

    try_merge_chunks(this_chunk);

   // assert(0 && "TODO cFree() unimplemented");
}

void* si_calloc(size_t size, int n)
{
    assert(0 && "TODO si_calloc() unimplemented");
}


