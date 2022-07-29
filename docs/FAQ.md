# Troubleshooting / FAQs

You may find solutions here for known issues.

If you didn't find your problem in the following list, please consider submit a
[GitHub issue](https://github.com/zenustech/zeno/issues) so that we can help you solve it.

> If you solved the problem yourself, you may create a pull Request to edit this file,
> adding your own solution (according to the existing format), so that later people with same
> problem can find solution easier.

## CMake problem

### Q

You ran CMake for multiple times, and CMake somehow failed with confusing error messages.

### A

CMake is a stupid state machine, the *cached* configuration variables from previous build may break the next build.

Try remove the `build` directory completely:

```bash
rm -rf build
```

Then re-run CMake again:

```
cmake -B build
cmake --build build --config Release
```

> JOKE: Every time you meet a problem about CMake, try `rm -rf build` first :) This solve 99% problems.
> If `rm -rf build` doesn't solve, this is the 1% real problems, ask us for help.

> See also my [public course](https://github.com/parallel101/course) for learning CMake (and C++ parallel programming).

## Windows problem

### Q

Zeno runs extremely slow, I believe this is abnormal.

### A

Consider quit anti-virus softwares like 360, they will put all console process into a virtualized sandbox, result in a very poor performance.

### Q

```
Error: Could not find 'Qt5Core.dll'
```

when running `build\bin\zenoedit.exe`.

### A

Suppose you have installed `Qt5` in `C:\Qt\Qt5.14.2`.
Then this file may be located in `C:\Qt\Qt5.14.2\msvc2017_64\bin\Qt5Core.dll`.
You have three solutions:

1. Manually copy the `Qt5Core.dll`, `Qt5Gui.dll`, and etc. to the `build\bin` directory (same directory as `zenoedit.exe` is).
2. Add the path `C:\Qt\Qt5.14.2\msvc2017_64\bin` (where `Qt5Core.dll` is located in) to the environment variable `PATH`.
3. Execute this in `cmd`: `C:\Qt\Qt5.14.2\msvc2017_64\bin\windeployqt.exe build\bin\zenoedit.exe`.

### Q

```
This application failed to start because it could not find or load the Qt platfo
rm plugin "windows".

Available platform plugins are: minimal, offscreen, windows
s).

Reinstalling the application may fix this problem.
```

### A

Also three solutions:

1. Manually copy the directory `C:\Qt\Qt5.14.2\msvc2017_64\plugins\platforms` to `build\bin\platforms`.
2. Add an environment variable `QT_QPA_PLATFORM_PLUGIN_PATH` with content `C:\Qt\Qt5.14.2\msvc2017_64\plugins\platforms`.
3. Execute this in `cmd`: `C:\Qt\Qt5.14.2\msvc2017_64\bin\windeployqt.exe build\bin\zenoedit.exe`.

See: https://stackoverflow.com/questions/34948960/qt-platform-plugin-windows-is-not-found-on-one-computer

### Q

```
error LNK2038: mismatch detected for 'RuntimeLibrary': value 'MDd_DynamicDebug' doesn't match value 'MD_DynamicRelease' in xxx.obj
```

### A

Please always use the `Release` configuration.

```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Both `-DCMAKE_BUILD_TYPE=Release` and `--config Release` are required.

> Delete the `build` directory completely (see CMake problem) whenever you restart the build.

> If the second step (`--config Release`) doesn't work, you need to open the `zeno.sln` in Visual Studio, and **select the `Release` configuration in the UI**, and click `Build`.

### Q

The compiler complains about encoding errors (GB2312 vs UTF-8).

### A

This is because Windows (Chinese edition) use GB2312 by default, but we use UTF-8 (for writing Chinese comments) in our code.

Please either add `add_compile_options("/utf-8")` at the head of `CMakeLists.txt` to make MSVC happy, or delete these useless comments that contains UTF-8 characters (related article: https://blog.csdn.net/u014671962/article/details/101525645).

## WSL problem

### Q

```
qt.qpa.xcb: could not connect to display
qt.qpa.plugin: Could not load the Qt platform plugin "xcb" in "" even though it was found.
This application failed to start because no Qt platform plugin could be initialized. Reinstalling the application may fix this problem.

Available platform plugins are: eglfs, linuxfb, minimal, minimalegl, offscreen, vnc, xcb.

Aborted (core dumped)
```

when running `build/bin/zenoedit`.

### A

This means your Linux system don't have a graphic interface (GUI).
You might be using WSL, **WSL don't have GUI by default**. They only have command line interface but no graphic interface.
So all GUI application won't work in your WSL. This is *not a Zeno bug*, but a *microsoft bug*.

My opinion: **never use WSL, Docker, or virtual machines**, they have extremely poor graphic support (no GPU).
For best performance, why not use native Windows instead? Zeno already have full-support for Windows years ago.
If you insist to use WSL, here's the solution:

1. You need to install X-Launch (or vcxsrv): https://sourceforge.net/projects/xlauncher/
2. Start X-Launch (select the multi-window mode if it asks).
3. Execute `export DISPLAY=:0` in the WSL terminal.
4. Now run Zeno again, its GUI window should shows up successfully.

> If you are WSL2, may need `export DISPLAY=192.168.xx.xx:0` instead, where `192.168.xx.xx` should be your local IP address.
> Some people says that WSL2 now have graphic interface by default, I'm not sure. I never used WSL2.

To test if your GUI is working, use the `xclock` (a small GUI application showing a clock):

```bash
sudo apt-get install -y xclock
xclock
```

* If the `xclock` failed to start: your WSL still doesn't have GUI, **all GUI application won't work**, not just Zeno.
* If the `xclock` starts successfully: your WSL GUI is working well, go ahead and Zeno should work too.

> Zeno still gives the same error even after `xclock` worked? Try `sudo apt-get install qt5-default`.

> Not using WSL but still meet this problem? You might be using Zeno on a headless-server, hint: `ssh -X root@yourserver.address`

## Ubuntu problem

### Q

```
/usr/include/c++/9/exception:143:10: fatal error: /mnt/e/zeno/build/ui/zenoui/zenoui_autogen/include/bits/exception_ptr.h: Invalid argument
  143 | #include <bits/exception_ptr.h>
      |          ^~~~~~~~~~~~~~~~~~~~~~
```

during `cmake --build build`.

### A

This is because your Qt5 and GCC libs version mismatched, upgrade them:

```bash
sudo apt-get update
sudo apt-get upgrade
rm -rf build           # clean cmake build directory and re-run cmake
```

### Q

```
/usr/lib/qt5/bin/uic: error while loading shared libraries: libQt5Core.so.5: cannot open shared object file: No such file or directory
```

during `cmake --build build`.

### A

This is a silly bug from Ubuntu official team, you need manually run this command to fix the stupid `libQt5Core.so.5`:

```bash
sudo strip --remove-section=.note.ABI-tag /usr/lib64/libQt5Core.so.5
```

Reference: https://askubuntu.com/questions/1034313/ubuntu-18-4-libqt5core-so-5-cannot-open-shared-object-file-no-such-file-or-dir

### Q

Any error around TBB.

### A

Consider uninstall OneAPI (`/opt/intel/oneapi`), OpenVDB requires the old version of TBB, not the stupid OneTBB.

## CUDA problem

### Q

CMake error when I set `-DZENO_WITH_CUDA:BOOL=ON`:

```
CMake Error at C:/Program Files/CMake/share/cmake-3.23/Modules/CMakeDetermineCompilerId.cmake:491 (message):
  No CUDA toolset found.
```

### A

Are you using Windows? Then you must install the **CUDA Visual Studio integration** first. And you cannot use MinGW.

### Q

The OptiX renderer failed to work on my computer.

### A

OptiX requires RTX20xx and above (hardware accelerated ray-tracing). And make sure you have installed NVIDIA's latest CUDA & OpenGL drivers.

Also check if you have **switched to `High-Performance`** in NVIDIA settings panel.
Laptop users usually have dual-cards, and Windows use the stupid Intel card by default (so called `Power-Saving`), which is extremely slow.
You need to switch to the `High-Performance` in the stupid Windows GUI settings, so that Windows will start Zeno with NVIDIA card by default.
