cmake_minimum_required(VERSION 3.16)

project(midim2)

set(enable-aufile OFF CACHE BOOL "compile support for sound file output" FORCE)
set(enable-ipv6 OFF CACHE BOOL "enable IPv6 support" FORCE)
set(enable-network OFF CACHE BOOL "enable network support (requires BSD sockets)" FORCE)
set(enable-signalsmith OFF CACHE BOOL "compile Limiter and reverb engine support (requires signalsmith-audio)" FORCE)
set(enable-sdl3 OFF CACHE BOOL "compile SDL3 audio support (if it is available)" FORCE)
set(enable-libsndfile OFF CACHE BOOL "compile libsndfile support (if it is available)" FORCE)
add_subdirectory(fluidsynth)

add_library(midim2 SHARED
    src/mp.h
    src/mp.c
    src/extension_exports.h
    src/extension_exports.c)

target_link_libraries(midim2 PRIVATE libfluidsynth)

# test program

add_executable(midim2test
    src/test_program/main.c)

configure_file(testfiles/test.sf2 . COPYONLY)
configure_file(testfiles/test.mid . COPYONLY)
configure_file(testfiles/test2.mid . COPYONLY)

target_link_libraries(midim2test PUBLIC midim2)
