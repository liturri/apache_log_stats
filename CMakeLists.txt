cmake_minimum_required(VERSION 3.0.2)
project(access_log_stats VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS ON)

if(CMAKE_BUILD_STATIC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

# Build with Profile enabled
if(CMAKE_BUILD_PROFILE)
    set(CMAKE_EXE_LINKER_FLAGS "-lprofiler")
    message(STATUS "Profiler link...")
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CURSES_NEED_NCURSES TRUE)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_subdirectory(libs)
add_subdirectory(libs-remote)
add_subdirectory(src)
