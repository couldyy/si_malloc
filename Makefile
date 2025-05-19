.PHONY: all clean install uninstall test

CC = gcc
AR = ar


SIMALLOC_LIBTYPE       ?= STATIC

#change to RELEASE for release builds
SIMALLOC_BUILD_MODE ?= DEBUG
#
# Determine if the file has root access (only required to install raylib)
# "whoami" prints the name of the user that calls him (so, if it is the root user, "whoami" prints "root")
ROOT = $(shell whoami)

DESTDIR ?= /usr/local
SIMALLOC_INSTALL_PATH ?= $(DESTDIR)/lib
# install path for header
SIMALLOC_H_INSTALL_PATH ?= $(DESTDIR)/include

ifeq ($(SIMALLOC_BUILD_MODE),DEBUG)
	SIMALLOC_BUILD_FLAGS = -Wall -Wextra -g -O0
else 
	SIMALLOC_BUILD_FLAGS = -Wall -Wextra -O3
endif

all: simalloc

#test: simalloc.c simalloc_utils.c simalloc_utils.h simalloc_internal.h main.c simalloc.h
test:
	rm -rf tests/build/
	mkdir tests/build
	#TODO create different tests
	#$(CC) -Wall -Wextra -g -O0 -o build/main main.c simalloc.c simalloc_utils.c

simalloc: include/simalloc.h simalloc.c simalloc_internal.h simalloc_utils.h simalloc_utils.c
#	rm -rf ./build/
#	mkdir build
	
ifeq ($(SIMALLOC_LIBTYPE), SHARED)
	$(CC) $(SIMALLOC_BUILD_FLAGS) -fPIC -o simalloc.o -c simalloc.c 
	$(CC) $(SIMALLOC_BUILD_FLAGS) -fPIC -o simalloc_utils.o -c simalloc_utils.c 
	$(CC) -o libsimalloc.so simalloc.o simalloc_utils.o -shared 
else
	$(CC) $(SIMALLOC_BUILD_FLAGS) -o simalloc.o -c simalloc.c 
	$(CC) $(SIMALLOC_BUILD_FLAGS) -o simalloc_utils.o -c simalloc_utils.c
	$(AR) rcs libsimalloc.a simalloc.o simalloc_utils.o
endif


install: 
ifeq ($(ROOT),root)
# Prepare the environment as needed.
	mkdir --parents --verbose $(SIMALLOC_INSTALL_PATH)
	mkdir --parents --verbose $(SIMALLOC_H_INSTALL_PATH)
ifeq ($(SIMALLOC_LIBTYPE),SHARED)
	cp --update --verbose libsimalloc.so $(SIMALLOC_INSTALL_PATH)/libsimalloc.so
else
	cp --update --verbose libsimalloc.a $(SIMALLOC_INSTALL_PATH)/libsimalloc.a
endif
	cp --update --verbose include/simalloc.h $(SIMALLOC_H_INSTALL_PATH)/simalloc.h
	@echo "simalloc development files installed/updated!"
else
	@echo "Error: Root permissions needed for installation. Try sudo make install"
endif

uninstall:
ifeq ($(ROOT),root)
ifeq ($(SIMALLOC_LIBTYPE), SHARED)
	rm --force --interactive --verbose $(SIMALLOC_INSTALL_PATH)/libsimalloc.so
else
	rm --force --interactive --verbose $(SIMALLOC_INSTALL_PATH)/libsimalloc.a
endif
	rm --force --interactive --verbose $(SIMALLOC_H_INSTALL_PATH)/simalloc.h
	@echo "simalloc development files removed!"
else
	@echo "Error: Root permissions needed for uinstallation. Try sudo make uinstall"
endif

clean:
	rm -fv libsimalloc.a libsimalloc.so *.o
	@echo "removed all generated files!"
