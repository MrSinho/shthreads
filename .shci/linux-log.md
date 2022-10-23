
# shthreads

![linux-badge](linux-exit_code.svg)

## [linux build logs:](https://github.com/mrsinho/shci)

        

Build ran for `10.71s`

---

```bash
sudo apt install -y gcc cmake make gdb libc6-dev
```

```bash
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.18.4-2+deb11u1).
gcc is already the newest version (4:10.2.1-1).
gdb is already the newest version (10.1-1.7).
libc6-dev is already the newest version (2.31-13+deb11u4).
make is already the newest version (4.3-4.1).
0 upgraded, 0 newly installed, 0 to remove and 0 not upgraded.

```

---

    
```bash
cd shthreads
mkdir build
cd build 
cmake .. -DSH_THREADS_BUILD_SAMPLES=ON
cmake --build .
```

```bash
-- The C compiler identification is GNU 10.2.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/c/Users/sino3/Desktop/shci-unix-test/shthreads/shthreads/build
Scanning dependencies of target shthreads
[ 12%] Building C object CMakeFiles/shthreads.dir/shthreads/src/shthreads.c.o
[ 25%] Linking C static library ../bin/libshthreads.a
[ 25%] Built target shthreads
Scanning dependencies of target shthreads-sample-semaphore
[ 37%] Building C object CMakeFiles/shthreads-sample-semaphore.dir/samples/src/shthreads-sample-semaphore.c.o
[ 50%] Linking C executable shthreads-sample-semaphore
[ 50%] Built target shthreads-sample-semaphore
Scanning dependencies of target shthreads-sample-mutex
[ 62%] Building C object CMakeFiles/shthreads-sample-mutex.dir/samples/src/shthreads-sample-mutex.c.o
[ 75%] Linking C executable ../bin/shthreads-sample-mutex
[ 75%] Built target shthreads-sample-mutex
Scanning dependencies of target shthreads-sample
[ 87%] Building C object CMakeFiles/shthreads-sample.dir/samples/src/shthreads-sample.c.o
[100%] Linking C executable ../bin/shthreads-sample
[100%] Built target shthreads-sample

```

---

    

Build terminated with exit code 0

---

