#ifndef _SI_MALLOC_INTERNAL_H
#define _SI_MALLOC_INTERNAL_H


#define DEBUG
//#define DEBUG_MALLOC
//#define DEBUG_FREE

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

// get arch and set alignment
#if defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)
    #define ALIGNMENT_SIZE 8
#elif defined(__i386__) || defined(_M_IX86) || defined(__arm__)
    #define ALIGNMENT_SIZE 4
#else 
    #define ALIGNMENT_SIZE 8    // TODO: handle unknown arch, for now, just make alignment 8 bytes
    #define UNKNOWN_ARCH
#endif
#define ALIGN(size) (((size) + (ALIGNMENT_SIZE - 1)) & ~(ALIGNMENT_SIZE - 1))

#define GET_METADATA_PTR(ptr) ((struct MemoryChunk*)((char*)ptr - sizeof(struct MemoryChunk)))

static uint64_t canary_secret = 0;


// si_rand()
#define SI_RAND_MAX 2147483647
static unsigned long si_rand_state = 1;



struct MemoryChunk {
   size_t size;
   struct MemoryChunk* next_chunk;
   struct MemoryChunk* prev_chunk;
   uint64_t canary;
};

//typedef struct {
//    void* page_ptr;
//    size_t page_size;
//    int offset;         
//                        
//
//    struct MemoryChunk* free_frames; 
//    size_t free_mem_size;
//
//    struct MemoryChunk* allocated_frames; 
//    size_t allocated_mem_size;
//
//} MemPage;


typedef struct {
    struct MemoryChunk* free_chunks; 

    struct MemoryChunk* allocated_chunks; 

} Arena;

extern Arena* memory_arena;

// didnt want to link with stdlib.h, so had to implenment those
void si_srand(unsigned long seed);
unsigned long my_rand();
//

Arena* init_ci_malloc();
void append_free_chunks_list(struct MemoryChunk* list, struct MemoryChunk* chunk);
void try_merge_chunks(struct MemoryChunk* this_chunk); 
void __d_print_free_chunks();

void init_canary();
uint64_t generate_canary(struct MemoryChunk* chunk_ptr, size_t chunk_size);
// XOR canaries, 0 returned when canaries are equal
int64_t compare_canaries(uint64_t canary_a, uint64_t canary_b);

#endif // _SI_MALLOC_INTERNAL_H
