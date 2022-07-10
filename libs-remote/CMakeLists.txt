include(FetchContent)

# #########################
# ##  Fetch: cxxcurses  ###
# #########################
#set(CXXCURSES_VERSION v0.0.1)
set(FMT_VERSION 8.1.1)

# FetchContent_Declare(cxxcurses
#     GIT_REPOSITORY https://github.com/hjaremko/cxxcurses.git
#     # GIT_TAG "${CXXCURSES_VERSION}"
# )

# message(STATUS "Fetching cxxcurses...")
# FetchContent_MakeAvailable(cxxcurses)


FetchContent_Declare(fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG "${FMT_VERSION}"
)

# FetchContent_Populate(fmt)
message(STATUS "Fetching fmt...")
FetchContent_MakeAvailable(fmt)
