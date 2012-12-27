
add_executable(mm_test_mono
    mono.c
    util.c
    )

install(TARGETS mm_test_mono DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
