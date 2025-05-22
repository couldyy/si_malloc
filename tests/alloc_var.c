#include <simalloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main()
{
    int* var = si_malloc(sizeof(int));
    *var = 125;
    printf("var: %d\n", *var);
    si_free(var);

    char* temp_str = "Hello world";
    char* heap_str = si_malloc(strlen(temp_str));
    strncpy(heap_str, temp_str, strlen(temp_str));
    printf("%s\n", heap_str);
    si_free(heap_str);

    return 0;
}

