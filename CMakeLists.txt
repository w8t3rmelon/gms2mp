cmake_minimum_required(VERSION 3.16)

project(gms2mp LANGUAGES C)

find_package(FluidSynth CONFIG REQUIRED)

add_library(gms2mp SHARED mp.h mp.c
    extension_exports.h
    extension_exports.c)

target_link_libraries(gms2mp PRIVATE FluidSynth::libfluidsynth)
