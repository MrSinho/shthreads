
# shthreads

![linux-badge](exit_code.svg)

## [linux build logs:](https://github.com/mrsinho/shci)

```
system        : Linux
version       : #1 SMP Fri Jan 27 02:56:13 UTC 2023
platform      : Linux-5.15.90.1-microsoft-standard-WSL2-x86_64-with-glibc2.31
processor     : 
min frequency : 0.0 MHz
max frequency : 0.0 MHz
cores         : 4
```

build ran for `40.88 s` and terminated with exit code `0`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y gcc cmake make libc6-dev
}

PREREQUISITES > .shci/linux/prerequisites-output.txt

```

```bash
Hit:1 http://deb.debian.org/debian bullseye InRelease
Get:2 http://security.debian.org/debian-security bullseye-security InRelease [48.4 kB]
Get:3 http://ftp.debian.org/debian bullseye-backports InRelease [49.0 kB]
Get:4 http://deb.debian.org/debian bullseye-updates InRelease [44.1 kB]
Get:5 http://security.debian.org/debian-security bullseye-security/main amd64 Packages [252 kB]
Fetched 394 kB in 2s (165 kB/s)
Reading package lists...
Building dependency tree...
Reading state information...
6 packages can be upgraded. Run 'apt list --upgradable' to see them.
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.18.4-2+deb11u1).
gcc is already the newest version (4:10.2.1-1).
libc6-dev is already the newest version (2.31-13+deb11u6).
make is already the newest version (4.3-4.1).
0 upgraded, 0 newly installed, 0 to remove and 6 not upgraded.

```

---
    
```bash
function BUILD {
	mkdir linux
    cd linux
    cmake -DSH_THREADS_BUILD_SAMPLES=ON ..
    cmake --build .
}

BUILD > .shci/linux/build-output.txt

```

```bash
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/Desktop/GitHub/shthreads/linux
[ 33%] Built target shthreads
Scanning dependencies of target shthreads-sample-mutex
[ 50%] Building C object CMakeFiles/shthreads-sample-mutex.dir/samples/src/shthreads-sample-mutex.c.o
[ 66%] Linking C executable ../bin/shthreads-sample-mutex
[ 66%] Built target shthreads-sample-mutex
[100%] Built target shthreads-sample

```

---
    
build ran for `40.88 s` and terminated with exit code `0`

---

