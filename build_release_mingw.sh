#!/bin/bash

cmake \
    -DCMAKE_TOOLCHAIN_FILE="cmake/mingw-linux-cross-compile.cmake" \
    -DCMAKE_BUILD_TYPE=Release \
    -GNinja \
    -Bout/mingw && \
    ninja -Cout/mingw -j12 && \
cp out/mingw/fluidsynth/src/libfluidsynth-3.dll out/mingw && \
strip out/mingw/libfluidsynth-3.dll && \
strip out/mingw/libmidim2.dll \
    -Kmp_set_use_system_patches \
    -Kmp_set_alt_sf_path \
    -Kmp_play \
    -Kmp_pause \
    -Kmp_resume \
    -Kmp_unload \
    -Kmp_seek \
    -Kmp_restart \
    -Kmp_set_bpm \
    -Kmp_get_bpm \
    -Kmp_get_division \
    -Kmp_get_tempo \
    -Kmp_get_length_ticks \
    -Kmp_get_pos_ticks \
    -Kmp_set_gain \
    -Kmp_get_gain \
    -Kmp_set_loop_count \
    -Kmp_wait_until_finished \
    -Kmp_init \
    -Kmp_setup \
    -Kmp_deinit
