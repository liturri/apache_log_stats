cmake_minimum_required(VERSION 3.0.2)
project(jpcre2 CXX)
project(jpcre2 VERSION 10.32.1)

add_library(jpcre2 INTERFACE)
target_include_directories(jpcre2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/jpcre2/src/)

