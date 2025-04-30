#include "simalloc_internal.h"
#include <unistd.h>


Arena* init_ci_malloc()
{
#ifdef UNKNOWN_ARCH
    fprintf(stderr, "Unknown architecture, memory alignment is set to %d bytes\n", ALIGNMENT_SIZE);
#endif
    Arena* arena_metadata_ptr = (Arena*)sbrk(sizeof(Arena));
    *arena_metadata_ptr = (Arena){NULL, NULL};
    return arena_metadata_ptr;
}

void append_free_chunks_list(struct MemoryChunk* free_chunks_list, struct MemoryChunk* chunk)
{
    struct MemoryChunk *temp_free_chunk = free_chunks_list;
    struct MemoryChunk *this_chunk = chunk;
// if list is empty
    if(temp_free_chunk == NULL) {
        this_chunk->next_chunk = NULL;
        this_chunk->prev_chunk = NULL;
        memory_arena->free_chunks= this_chunk;
        return;
    } 
    // if this_chunk < head_of_list
    else if(this_chunk <= memory_arena->free_chunks) {     
        memory_arena->free_chunks->prev_chunk = this_chunk;
        this_chunk->next_chunk = memory_arena->free_chunks;
        this_chunk->prev_chunk = NULL;
        memory_arena->free_chunks = this_chunk;
        return;
    }

    struct MemoryChunk* temp_prev_item = NULL;
    while(temp_free_chunk != NULL && (this_chunk > temp_free_chunk)) {
        temp_prev_item = temp_free_chunk;
        temp_free_chunk = temp_free_chunk->next_chunk;
    }


    //temp_free_chunk now contains chunk, BEFORE which insert this_chunk
    //insert chunk
    if(temp_free_chunk == NULL) {   // end of the list
        this_chunk->next_chunk = NULL;
        this_chunk->prev_chunk = temp_prev_item;
        temp_prev_item->next_chunk = this_chunk;
    }
    else {
    //if(temp_free_chunk->prev_chunk != NULL)
        temp_free_chunk->prev_chunk->next_chunk = this_chunk;   // WARNING: may cause issues because of the order
        this_chunk->next_chunk = temp_free_chunk;
        this_chunk->prev_chunk = temp_free_chunk->prev_chunk;
        temp_free_chunk->prev_chunk = this_chunk;
    }

}

void try_merge_chunks(struct MemoryChunk* this_chunk)
{

    //try to merge with left chunk
    // if previous chunk is neighbor to the left(this_chunk - (size of prev_chunk + size of metadata) = prev_chunk)
    if(this_chunk->prev_chunk != NULL && 
            ((struct MemoryChunk*)((char*)this_chunk - (this_chunk->prev_chunk->size + sizeof(struct MemoryChunk))) == this_chunk->prev_chunk)) {    // for some reason doesnt work
        this_chunk->prev_chunk->size += (this_chunk->size + sizeof(struct MemoryChunk));    // add to prev_chunk->size, size of current chunk + size of current chunk metadata
        this_chunk->prev_chunk->next_chunk = this_chunk->next_chunk; 
        if(this_chunk->next_chunk != NULL) {
            this_chunk->next_chunk->prev_chunk = this_chunk->prev_chunk;
        }
#ifdef DEBUG
        printf("sizeof new merged chunk to the left: %zu\n", this_chunk->prev_chunk->size);
#endif
        this_chunk = this_chunk->prev_chunk;
    }
    // try to merge with rigth chunk
    // if next chunk is neighbor to the right (this_chunk + size == next_chunk)
    if(this_chunk->next_chunk != NULL && ((struct MemoryChunk*)((char*)this_chunk + this_chunk->size + sizeof(struct MemoryChunk)) == this_chunk->next_chunk)) {
        this_chunk->size += (this_chunk->next_chunk->size + sizeof(struct MemoryChunk));    //add to chunk->size size of next_chunk + size of metadata
        if(this_chunk->next_chunk->next_chunk != NULL)
            this_chunk->next_chunk->next_chunk->prev_chunk = this_chunk;
        this_chunk->next_chunk = this_chunk->next_chunk->next_chunk;
#ifdef DEBUG
        printf("sizeof new merged chunk to the right: %zu\n", this_chunk->size);
#endif
        //this_chunk = this_chunk->prev_chunk;
    }
}

void __d_print_free_chunks() 
{
    printf("------------------------------\n");
    struct MemoryChunk* temp = memory_arena->free_chunks;
    while(temp != NULL) {
        printf("chunk: %p, %d\n", temp, (temp->next_chunk != NULL) ? (temp < temp->next_chunk) : 0);
        temp = temp->next_chunk;
    }
}
