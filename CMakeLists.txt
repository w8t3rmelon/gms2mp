cmake_minimum_required(VERSION 3.16)

project(gms2mp LANGUAGES C)

find_package(FluidSynth CONFIG REQUIRED)

add_library(gms2mp SHARED
    src/mp.h
    src/mp.c
    src/extension_exports.h
    src/extension_exports.c)

target_link_libraries(gms2mp PRIVATE FluidSynth::libfluidsynth)