#include <stdio.h>
#include "simalloc.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    char a;
    int b;
} test;

int main() {
    /*
    int* arr = (int*)si_malloc(sizeof(int) * 10);
    for(int i = 0; i < 10; i++) {
        arr[i-1] = i;
        printf("ptr: %p, data: %d\n", &arr[i-1], arr[i-1]);
    }
    for(int i = 0; i < 10; i++) {
        printf("ptr: %p, data: %d\n", &arr[i-1], arr[i-1]);
    }
    printf("%d\n", *arr);
    printf("%d\n", arr[7]);
    si_free(arr);
    printf("%d\n", arr[7]);
   */ 
//    int* var1 = si_malloc(sizeof(int) * 2);
//    int* var2 = si_malloc(sizeof(int) * 3);
//    (void)sbrk(10);
//    int* var3 = si_malloc(sizeof(int) * 1);
//    (void)sbrk(10);
//    si_free(var1);
//    si_free(var3);
//    si_free(var2);
//    printf("%d\n", *var1);

    /*
    char* var1 = si_malloc(sizeof(char));
    char* var2 = si_malloc(sizeof(char) * 3);
    (void)sbrk(10);
    char* var3 = si_malloc(sizeof(char) * 2);
    char* var4 = si_malloc(sizeof(char) * 8);
    //(void)sbrk(10);
    char* var5 = si_malloc(sizeof(char) * 9);
    (void)sbrk(10);
    si_free(var1);
    __d_print_free_chunks();
    si_free(var3);
    __d_print_free_chunks();
    si_free(var2);
    __d_print_free_chunks();
    si_free(var4);
    __d_print_free_chunks();
    si_free(var5);
    __d_print_free_chunks();
    //int* var1 = si_malloc(sizeof(int));
    // *var1 = 10;
    //si_free(var1);
    //printf("%d\n", *var1);
    */

    int *var1 = si_malloc(sizeof(int));
    *var1 = 10;
    si_free(var1);
    sbrk(sizeof(int) * 20);
    si_free(var1);
    return 0;
}
