add_executable(access_log_stats main.cc log_stats.cc follow.cc term.cc app.cc config.cc summaries.cc logline.cc)
target_link_libraries(access_log_stats PUBLIC fmt::fmt PUBLIC ${CURSES_LIBRARIES} )

if(CMAKE_BUILD_STATIC)
    set_target_properties(access_log_stats PROPERTIES LINK_SEARCH_START_STATIC 1)
    set_target_properties(access_log_stats PROPERTIES LINK_SEARCH_END_STATIC 1)
endif()

add_executable(test_follow test.cc follow.cc)