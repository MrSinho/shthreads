
# shthreads

![windows-badge](windows-exit_code.svg)

## [windows build logs:](https://github.com/mrsinho/shci)

        

Build ran for `37.14s`

---

```bash
choco install -y mingw cmake
mingw-get install gcc mingw32-make gdb
```

```bash
Chocolatey v0.11.2
Chocolatey detected you are not running from an elevated command shell
 (cmd/powershell).

 You may experience errors - many functions/packages
 require admin rights. Only advanced users should run choco w/out an
 elevated shell. When you open the command shell, you should ensure
 that you do so with "Run as Administrator" selected. If you are
 attempting to use Chocolatey in a non-administrator setting, you
 must select a different location other than the default install
 location. See
 https://docs.chocolatey.org/en-us/choco/setup#non-administrative-install
 for details.

For the question below, you have 20 seconds to make a selection.

 Do you want to continue?([Y]es/[N]o): 
Timeout or your choice of '' is not a valid selection.
Installing the following packages:
mingw;cmake
By installing, you accept licenses for the packages.
mingw v11.2.0.07112021 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
cmake v3.24.2 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

Chocolatey installed 0/2 packages. 
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).

Warnings:
 - cmake - cmake v3.24.2 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - mingw - mingw v11.2.0.07112021 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

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
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.22621.0 to target Windows 10.0.21996.
-- The C compiler identification is MSVC 19.32.31332.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.32.31326/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: C:/Users/sino3/Desktop/shci-test/shthreads/shthreads/build
Microsoft (R) Build Engine version 17.2.1+52cd2da31 for .NET Framework
Copyright (C) Microsoft Corporation. All rights reserved.

  Checking Build System
  Building Custom Rule C:/Users/sino3/Desktop/shci-test/shthreads/shthreads/CMakeLists.txt
  shthreads.c
  shthreads.vcxproj -> C:\Users\sino3\Desktop\shci-test\shthreads\shthreads\bin\Debug\shthreads.lib
  Building Custom Rule C:/Users/sino3/Desktop/shci-test/shthreads/shthreads/CMakeLists.txt
  shthreads-sample.c
  shthreads-sample.vcxproj -> C:\Users\sino3\Desktop\shci-test\shthreads\shthreads\bin\Debug\shthreads-sample.exe
  Building Custom Rule C:/Users/sino3/Desktop/shci-test/shthreads/shthreads/CMakeLists.txt
  shthreads-sample-mutex.c
  shthreads-sample-mutex.vcxproj -> C:\Users\sino3\Desktop\shci-test\shthreads\shthreads\bin\Debug\shthreads-sample-mutex.exe
  Building Custom Rule C:/Users/sino3/Desktop/shci-test/shthreads/shthreads/CMakeLists.txt
  shthreads-sample-semaphore.c
  shthreads-sample-semaphore.vcxproj -> C:\Users\sino3\Desktop\shci-test\shthreads\shthreads\build\Debug\shthreads-sample-semaphore.exe
  Building Custom Rule C:/Users/sino3/Desktop/shci-test/shthreads/shthreads/CMakeLists.txt

```

---

    

Build terminated with exit code 0

---

