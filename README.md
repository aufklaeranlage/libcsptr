C Smart Pointers
================


## What this is

This project is a modified copy of the [csptr library](https://github.com/Snaipe/libcsptr) by [Snaipe](https://github.com/Snaipe), that removed a couple of GNU specific compiler attributes and meta data for the pointers.

It is focuse on cleaner management of memory access by differnetiating between `shared` and `unique` pointers.

This project is an attempt to bring smart pointer constructs
to the (GNU) C programming language.

### Features

* `unique_ptr` and `shared_ptr` macros
* Destructor support for cleanup

## Installing

### Building from source
#### Prerequisites

TODO: Check with older compiler version

#### Installation

1. Clone this repository
2. run `mkdir build && cd $_ && cmake -DCMAKE_INSTALL_PREFIX=$HOME .. && make && make install`  
   from the project root for a local install, or run  
   `mkdir build && cd $_ && cmake -DCMAKE_INSTALL_PREFIX=/usr .. && make && sudo make install` for a global install.

In case you want to run the test available for this project you need to have the [check](https://github.com/libcheck/check) library installed. In case it is not installed in your system folder add -DCHECK_INSTALL_DIR=/path/to/check/install/dir when calling CMake.

## Examples

* Simple unique\_ptr:
    simple1.c:
    ```c
    #include <stdio.h>
    #include <csptr/smart_ptr.h>

    int main(void) {
      // some_int is an unique_ptr to an int.
      // It doesn't have a specific destructor set.
      int *some_int = unique_ptr(sizeof(int));
      *some_int = 1;

      printf("%p = %d", some_int, *some_int);

      sfree(some_int);
      return 0;
    }
    ```
    Shell session:
    ```bash
    $ gcc -std=c99 -o simple1 simple1.c -lcsptr
    $ valgrind ./simple1
    ==34600== Memcheck, a memory error detector
    ==34600== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==34600== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
    ==34600== Command: ./simple1
    ==34600==
    0x4aa0068 = 1==34600==
    ==34600== HEAP SUMMARY:
    ==34600==     in use at exit: 0 bytes in 0 blocks
    ==34600==   total heap usage: 2 allocs, 2 frees, 1,072 bytes allocated
    ==34600==
    ==34600== All heap blocks were freed -- no leaks are possible
    ==34600==
    ==34600== For lists of detected and suppressed errors, rerun with: -s
    ==34600== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
    ```
* Simple unique\_ptr with destructor:
    ```c
    #include <unistd.h>
    #include <fcntl.h>
    #include <csptr/smart_ptr.h>

    struct log_file {
      int fd;
      // ...
    };

    void cleanup_log_file(void *ptr)
    {
      close(((struct log_file *)ptr)->fd);
    }

    int main(void)
    {
      struct log_file *log = unique_ptr(sizeof(struct log_file), cleanup_log_file);
      log->fd = open("/dev/null", O_WRONLY | O_APPEND);
      write(log->fd, "Hello", 5);
      
      // cleanup_log_file is called by sfree, then log is freed
      sfree(log);
      return 0;
    }
    ```
    Shell session:
    ```bash
    $ gcc -std=c99 -o dtor dtor.c -lcsptr
    $ valgrind --track-fds=yes ./dtor
    ==39307== Memcheck, a memory error detector
    ==39307== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==39307== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
    ==39307== Command: ./dtor
    ==39307==
    ==39307==
    ==39307== FILE DESCRIPTORS: 3 open (3 std) at exit.
    ==39307==
    ==39307== HEAP SUMMARY:
    ==39307==     in use at exit: 0 bytes in 0 blocks
    ==39307==   total heap usage: 1 allocs, 1 frees, 48 bytes allocated
    ==39307==
    ==39307== All heap blocks were freed -- no leaks are possible
    ==39307==
    ==39307== For lists of detected and suppressed errors, rerun with: -s
    ==39307== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
    ```

## More examples

* Using a different memory allocator (although most will replace malloc/free):
    ```c
    #include <csptr/smart_ptr.h>

    void *some_allocator(size_t);
    void some_deallocator(void *);

    int main(void) {
        smalloc_allocator = (s_allocator) {some_allocator, some_deallocator};
        // ...
        return 0;
    }
    ```
