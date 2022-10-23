# shthreads

[![linux_build](.shci/linux-exit_code.svg)](.shci/linux-log.md) [![windows-build](.shci/windows-exit_code.svg)](.shci/windows-log.md)

A simple Windows and Unix threading library using the win32 api and posix, written in C.

## Features
 * Threads handling
 * Mutex synchronization

## Build

Tested with `gcc` and `MSVC`. The project files and `Makefile`s are generated with cmake:

```bash
git clone https://github.com/mrsinho/shthreads.git
cd shthreads
mkdir build && cd build
cmake .. #-DSH_THREADS_BUILD_SAMPLES=ON
```

## Example code

To build the samples, set on the cmake option `SH_THREADS_BUILD_SAMPLES`. 

See [`/samples/src/`](samples/src/) to find some reference source code:
 * [`shthreads-sample.c`](samples/src/shthreads-sample.c);
 * [`shthreads-sample-mutex.c`](samples/src/shthreads-sample-mutex.c).
