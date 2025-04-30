#ifndef _SI_MALLOC_INTERNAL_H
#define _SI_MALLOC_INTERNAL_H


#define DEBUG
//#define DEBUG_MALLOC
//#define DEBUG_FREE

#include <stdio.h>

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

struct MemoryChunk {
   size_t size;
   struct MemoryChunk* next_chunk;
   struct MemoryChunk* prev_chunk;
};

typedef struct {
    void* page_ptr;
    size_t page_size;
    int offset;         // because i need to store all of those structures
                        // i also need to occupie some space in a first

    struct MemoryChunk* free_frames; 
    size_t free_mem_size;

    struct MemoryChunk* allocated_frames; 
    size_t allocated_mem_size;

} MemPage;


typedef struct {
    struct MemoryChunk* free_chunks; 

    struct MemoryChunk* allocated_chunks; 

} Arena;

extern Arena* memory_arena;

#endif // _SI_MALLOC_INTERNAL_H
