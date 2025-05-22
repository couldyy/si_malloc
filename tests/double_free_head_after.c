#include <simalloc.h>


int main() {
    int *var1 = si_malloc(sizeof(int));     //<-- alloc memory using sbrk()
    *var1 = 10;                             //                                                                                                                  }
    si_free(var1);                          //<-- free memory using sbrk(-size), now var1 > sbrk(0)
    sbrk(sizeof(int) * 20);                 //<-- simulation of some function allocating memory using sbrk(), now var1 < sbrk() but we didnt allocate any memory
    si_free(var1);                          //<-- canary mismatch           
    return 0;
}

