cmake_minimum_required(VERSION 3.0.2)
project(jpcre2 CXX)
project(jpcre2 VERSION 10.32.1)

include(FetchContent)

set(PCRE2_SUPPORT_LIBREADLINE FALSE)

FetchContent_Declare(pcre2
    GIT_REPOSITORY https://github.com/Maluuba/pcre2
    GIT_TAG "pcre2-10.33"
)

#FetchContent_Populate(pcre2)
message(STATUS "Fetching pcre2...")
FetchContent_MakeAvailable(pcre2)
target_include_directories(pcre2-8 PUBLIC ${PCRE2_BINARY_DIR})


add_library(jpcre2 INTERFACE)
target_include_directories(jpcre2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/jpcre2/src/)
#target_link_libraries(jpcre2 PUBLIC pcre2-8)
#target_link_libraries(jpcre2 INTERFACE pcre2)