# How to build Zeno from source

## Software requirements

To work with Zeno, you need:
```cpp
Git >= 2.0 && CMake >= 3.18 && Qt == 5 && (MSVC >= 2019 || GCC >= 11 || Clang >= 12) && (Windows || Linux) && 64bit
```

### Windows

1. Download and install Git: https://git-scm.com/download/win

2. Download and install CMake: https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-windows-x86_64.zip

3. Download and install Visual Studio 2019 Community Edition (which is free): https://visualstudio.microsoft.com/zh-hans/downloads/

> Note that we install Visual Studio **only to get the compiler bundled with it**. feel free to use your favorite editors like VSCode or CLion other than Visual Studio for coding.

> It's recommended to install Visual Studio in trivial locations, like `C:/Programs Files (x86)/xxx` or at least `D:/Program Files (x86)/xxx`, so that VCPKG can find it easier.

4. Download and install Qt via their online installer: https://www.qt.io/download-qt-installer

> You need to click the `Qt 5.12.11` option to install the prebuilt Qt binaries, otherwise only the Qt Creator is selected by default.

> If you find installing Qt troublesome (e.g., asking you for registering a Qt account), feel free to skip it for now, and execute `vcpkg install qt5:x64-windows` later (see the `vcpkg` section) instead.

### Ubuntu

```bash
sudo apt-get install -y git cmake g++
sudo apt-get install -y qt5-default
```

### Arch Linux

```bash
sudo pacman -S git cmake g++
sudo pacman -S qt5
```

## Install required libraries (optional)

Zeno depends on many C++ libraries, we recommend to use the cross-platform package manager `vcpkg` to install these requirements. (feel free to follow your own favor as long as the requirements below can be installed :)

First of all, download `vcpkg`:

```bash
git clone https://github.com/microsoft/vcpkg.git --depth=1
cd vcpkg
```

Next, let's install Zeno dependencies via `vcpkg`:

### Windows (cmd)

```cmd
bootstrap-vcpkg.bat
vcpkg integrate install
vcpkg install openvdb:x64-windows
vcpkg install eigen3:x64-windows
vcpkg install cgal:x64-windows
vcpkg install openblas:x64-windows
vcpkg install lapack:x64-windows
```

> Windows users may need to **install the `English Pack` of Visual Studio** for VCPKG to work.

> Note that the `x64-windows` suffix **must be specified** otherwise VCPKG will install 32-bit packages by default :)

> For Chinese users, you may also need to follow the instruction in [this zhihu post](https://zhuanlan.zhihu.com/p/383683670) to **switch to domestic source** for faster download.

### Linux (bash)

```bash
./bootstrap-vcpkg.sh
./vcpkg install openvdb:x64-linux
./vcpkg install eigen3:x64-linux
./vcpkg install cgal:x64-linux
./vcpkg install openblas:x64-linux
./vcpkg install lapack:x64-linux
```

## Building Zeno from Source

Now that all requirements are satisfied, let's clone Zeno from GitHub:

```bash
git clone https://github.com/zenustech/zeno.git
cd zeno
```

Configure CMake:

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

> Please **replace the `[path to vcpkg]` by the path where we previously cloned vcpkg**, for example: `C:/Users/Administrator/vcpkg`, with `\\` replaced by `/` **even if you are on Windows** to make CMake happy.

> Remove the `-DCMAKE_TOOLCHAIN_FILE=xxx` flag if you **skipped vcpkg requirements installation**, ZENO supports build with zero-dependency as well.

> CLion users may also want to append the `-GNinja` option for parallel build. (`MSBuild` only use 1 CPU core for CMake targets...)

Starts to build (`8` here means using 8 CPU cores):

```bash
cmake --build build --parallel 8
```

## Run Zeno

### Windows (cmd)

```cmd
build\zeno.exe
```

### Linux (bash)

```bash
build/zeno
```

## References

- [VCPKG user guide](https://github.com/microsoft/vcpkg/blob/master/README_zh_CN.md)
- [hipSYCL install guide](https://github.com/illuhad/hipSYCL/blob/develop/doc/installing.md)
- [CMake documentation](https://cmake.org/cmake/help/latest/)
- [Git documentation](https://git-scm.com/doc)
- [C++ references](https://en.cppreference.com/w/)
- [Qt5 documentation](https://doc.qt.io/qt-5/)
- [OpenVDB cookbook](https://www.openvdb.org/documentation/doxygen/codeExamples.html)
- [SYCL specification](https://www.khronos.org/registry/SYCL/specs/sycl-2020/html/sycl-2020.html)
- [Zeno bug report](https://github.com/zenustech/zeno/issues)