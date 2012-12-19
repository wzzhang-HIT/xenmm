include_directories(
	.
)
add_library(xenstat STATIC
   libxenstat/xenstat.c
   libxenstat/xenstat_linux.c    
)

add_library(util STATIC
    mmstat.c
    mmstore.c
)


add_executable(mmclient
   mmclient.c
)

target_link_libraries(mmclient
    util
    xenstore
)




