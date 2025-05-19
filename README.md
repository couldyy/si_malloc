# simalloc 

> [!WARNING]
> This library is not finished!

This is a simple implementation of malloc that uses sbrk() for allocaion memory and mmap for big pages(will be implemented in future).\
No thread safety, no memory protection whatsoever. 

## Build
For now it is linux-only malloc (and probably will stay like that forever).\
So in order to compile a static version of library you have to
```
> git clone https://github.com/couldyy/si_malloc.git
> cd si_malloc
> make
> sudo make install
```
If you want to compile a shared version for dynamic linking, do
```
> SIMALLOC_LIBTYPE=SHARED make
> sudo SIMALLOC_LIBTYPE=SHARED make install
```
You can also configure installation path by specifying `DESTDIR ` (default is `usr/local`) 

For uninstallation of static version of library run
```
> sudo make uninstall
```
And for the dynamic version
```
> sudo SIMALLOC_LIBTYPE=SHARED make uinstall
```

For cleaning run `make clean`

## Usage
Just as a default malloc 
```
si_malloc(size_t size, int n)
si_free(void* ptr)

// Not implemented yet
//si_calloc()
//si_realloc()
```


