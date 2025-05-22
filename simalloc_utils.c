#include "simalloc_internal.h"


Arena* init_ci_malloc()
{
#ifdef UNKNOWN_ARCH
    fprintf(stderr, "Unknown architecture, memory alignment is set to %d bytes\n", ALIGNMENT_SIZE);
#endif
    si_srand(time(NULL));
    init_canary();
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


void si_srand(unsigned long seed)
{
    si_rand_state = seed;
}

unsigned long si_rand()
{
    si_rand_state = (si_rand_state * 1103515245 + 12345) % SI_RAND_MAX;
    return si_rand_state;
}

void init_canary() {
    canary_secret = ((uint64_t)si_rand() << 32) | si_rand();
}

uint64_t generate_canary(struct MemoryChunk* chunk_ptr, size_t chunk_size)
{
    intptr_t addr = (uintptr_t)chunk_ptr;

    // Mix address, size, and the global secret into a 64-bit canary
    uint64_t canary = addr ^ (chunk_size << 32 | chunk_size >> 32) ^ canary_secret;
#ifdef DEBUG_CANARY
    printf("generating chunk, addr: %p, chunk_size: %zu, canary:%p\n", addr, chunk_size, canary);
#endif
    canary ^= (canary >> 33) ^ (canary << 17);  // basic bit-mixing 
    return (canary_secret ^ (uintptr_t)chunk_ptr);
}

// XOR canaries, 0 returned when canaries are equal
inline int64_t compare_canaries(uint64_t canary_a, uint64_t canary_b)
{
    return (canary_a ^ canary_b);
}
