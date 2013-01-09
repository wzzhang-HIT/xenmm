
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
add_executable(mm_util_swap
    swap.c
    )

install(TARGETS 
    mm_test_mono 
    mm_test_random 
    mm_test_static
    mm_util_swap
    
    DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
