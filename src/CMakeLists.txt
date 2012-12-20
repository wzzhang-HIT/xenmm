include_directories(
	.
)
if(NOT ONLY_CLIENT)

add_library(xenstat STATIC
   libxenstat/xenstat.c
   libxenstat/xenstat_linux.c    
)

add_library(util STATIC
    mmstat.c
    mmstore.c
)

add_executable(mmserver
    mmserver.c
    )

target_link_libraries(mmserver
    util
    xenstore
    )

install(TARGETS mmserver DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

endif(NOT ONLY_CLIENT)



add_executable(mmclient
   mmclient.c
   mmstore.c
)

target_link_libraries(mmclient
    xenstore
)



install(TARGETS mmclient DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
