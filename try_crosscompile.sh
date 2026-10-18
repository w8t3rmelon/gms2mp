#!/bin/bash

mingw64-cmake \
    -DMINGW_LINUX_CROSS_COMPILE=ON \
    -DCMAKE_SHARED_LIBRARY_PREFIX="" \
    -DCMAKE_SHARED_LIBRARY_SUFFIX=".dll" \
    -DCMAKE_BUILD_TYPE=Release \
    -DVCPKG_TARGET_TRIPLET=x64-mingw-dynamic \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
    -G"Unix Makefiles" \
    -Bout/build; mingw64-make -Cout/build -j12
# cmake doesn't copy this automatically for some reason ?
cp out/build/vcpkg_installed/x64-mingw-dynamic/bin/libfluidsynth-3.dll out/build/
wine out/build/midim2test.exe
