cmake_minimum_required(VERSION 3.16)

project(midim2)

set(CACHE MINGW_LINUX_CROSS_COMPILE BOOL OFF)

if (MINGW_LINUX_CROSS_COMPILE)
    include(cmake/mingw-linux-cross-compile.cmake)
endif()

find_package(FluidSynth CONFIG REQUIRED)

add_library(midim2 SHARED
    src/mp.h
    src/mp.c
    src/extension_exports.h
    src/extension_exports.c)

target_link_libraries(midim2 PRIVATE FluidSynth::libfluidsynth)

# test program

add_executable(midim2test
    src/test_program/main.c)

configure_file(testfiles/test.sf2 . COPYONLY)
configure_file(testfiles/test.mid . COPYONLY)
configure_file(testfiles/test2.mid . COPYONLY)

target_link_libraries(midim2test PUBLIC midim2)
