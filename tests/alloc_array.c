#include <simalloc.h>
#include <stdio.h>

#define ARRAY_SIZE 10

void print_array(int* arr, int size);
void fill_array(int* arr, int size);

int main()
{
    int* arr = si_malloc(sizeof(int) * ARRAY_SIZE);
    fill_array(arr, ARRAY_SIZE);
    print_array(arr, ARRAY_SIZE);
    
    si_free(arr);
    print_array(arr, ARRAY_SIZE);
    si_free(arr);       // <- double free detected (canary mismatch)
    return 0;
}

void fill_array(int* arr, int size)
{
    for(int i = 0; i < size; i++) {
        arr[i] = size - i;
    }
}

void print_array(int* arr, int size)
{
    for(int i = 0; i < size; i++) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }
}

