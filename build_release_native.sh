#!/bin/bash

cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -GNinja \
    -Bout/native && \
ninja -Cout/native -j12 && \
cp out/native/fluidsynth/src/libfluidsynth.so.3 out/native/ && \
strip out/native/libfluidsynth.so.3 && \
strip out/native/libmidim2.so \
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
