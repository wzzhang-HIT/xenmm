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

endif(NOT ONLY_CLIENT)



add_executable(mmclient
   mmclient.c
   mmstore.c
)

target_link_libraries(mmclient
    xenstore
)




