/* 
    simalloc - Simple implementation of malloc
    really simple, low performance, linux only malloc made ONLY FOR EDUCATIONL PURPOSES

WARNING: DO NOT USE IN A REAL PROJECT
it is just a pet project there is NO thread safety, memory safety. It is not cross-platform and never will be */


/*--------------------------------------------------------------------------------
TODO:
[x] - done, [t] - done, but needs testing, [ ] - not done

[t] Make si_free() put freed chunks in sorted order, so in future can merge chunks
[t] Provide functionality for si_free() so it will merge neighbor chunks
[ ] Make si_malloc() look for free_chunks list with 'best fit' order
[ ] Provide functionality for si_malloc() if found chunk is 
     bigger enough(at least sizeof(MemoryChunk) + ALIGNMENT_SIZE bytes) - allocate part of it and put remainig part to the free list
[ ] If it possible - provide some protection for metadata, so user cant that easily access it
[ ] Implement si_calloc()
[ ] Implement si_realloc()
[ ] si_malloc(): if requested size is > 128kb, use mmap() and provide functionality to handle it(append Arena struct)
[ ] si_malloc(): check how much heap memory available left
[t] si_malloc(): provide memory alignment
[ ] si_free(): provide good double free protection
[ ] provide 'use after free' protection      (*for now it is just memset() to 0)
 
 --------------------------------------------------------------------------------*/

#ifndef _C_MALLOC_
#define _C_MALLOC_
#include <stddef.h>
#include "../simalloc_utils.h"
#include "../simalloc_internal.h"


void* si_malloc(size_t size);

void* si_calloc(size_t size, int n);

void si_free(void* ptr);



#endif //_C_MALLOC_
