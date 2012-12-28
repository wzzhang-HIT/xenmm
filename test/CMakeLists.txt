
add_executable(mm_test_mono
    mono.c
    util.c
    )

add_executable(mm_test_random
    random.c
    util.c
    )

add_executable(mm_test_static
    static.c
    util.c
    )

install(TARGETS 
    mm_test_mono 
    mm_test_random 
    mm_test_static
    
    DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
