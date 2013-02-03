include_directories(
	.
)

add_definitions(-Wall -W)

if(NOT ONLY_CLIENT)

    #add_library(xenstat STATIC
    #  libxenstat/xenstat.c
    #libxenstat/xenstat_linux.c    
    #)

add_library(util STATIC
    #mmstat.c
    mmstore.c
    mmctrl.c
    solve.c
    mmrecord.c
)

add_executable(mmserver
    mmserver.c
    type.c
    )

target_link_libraries(mmserver
    util
    xenstore
    xenctrl
    )
if(DEFINED TAX_RATE)
    set_target_properties(mmserver
        PROPERTIES
        COMPILE_FLAGS "-DTAX_RATE=${TAX_RATE}"
        )
endif(DEFINED TAX_RATE)

install(TARGETS mmserver DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

endif(NOT ONLY_CLIENT)



add_executable(mmclient
   mmclient.c
   mmstore.c
   type.c
)

target_link_libraries(mmclient
    xenstore
)



install(TARGETS mmclient DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
