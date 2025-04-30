#ifndef _SIMALLOC_UTILS_H
#define _SIMALLOC_UTILS_H
#include "simalloc_internal.h"

Arena* init_ci_malloc();
void append_free_chunks_list(struct MemoryChunk* list, struct MemoryChunk* chunk);
void try_merge_chunks(struct MemoryChunk* this_chunk); 
void __d_print_free_chunks();



#endif //_SIMALLOC_UTILS_H
