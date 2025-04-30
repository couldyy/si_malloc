
all: test

test: simalloc.c simalloc_utils.c simalloc_utils.h simalloc_internal.h main.c simalloc.h
	mkdir build
	gcc -Wall -Wextra -g -O0 -o build/main main.c simalloc.c simalloc_utils.c

simalloc: simalloc.c simalloc_utils.c
	mkdir build
	gcc -Wall -Wextra -g -O0 -o build/main main.c simalloc.c 
