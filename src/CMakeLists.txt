include_directories(
	.
)

add_definitions(-Wall -W)

if(NOT ONLY_CLIENT)

add_library(util STATIC
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
    m
    )
set(MMSERVER_COMPILE_FLAGS "")
if(DEFINED TAX_RATE)
    if(TAX_RATE STREQUAL "AUTO")
        set(MMSERVER_COMPILE_FLAGS "${MMSERVER_COMPILE_FLAGS} -DAUTO_TAX_RATE=1 ")
        message("[TAX_RATE = AUTO]")
    else(TAX_RATE STREQUAL "AUTO")
        set(MMSERVER_COMPILE_FLAGS "${MMSERVER_COMPILE_FLAGS} -DTAX_RATE=${TAX_RATE} ")
        message("[TAX_RATE = ${TAX_RATE}]")
    endif(TAX_RATE STREQUAL "AUTO")
    set_target_properties(mmserver
        PROPERTIES
        COMPILE_FLAGS "${MMSERVER_COMPILE_FLAGS}"
        )
endif(DEFINED TAX_RATE)

if(SOCK)
    set(MMSERVER_COMPILE_FLAGS "${MMSERVER_COMPILE_FLAGS} -DENABLE_SOCK=1 ")
    set_target_properties(mmserver
        PROPERTIES
        COMPILE_FLAGS "${MMSERVER_COMPILE_FLAGS}"
        )
    target_link_libraries(mmserver pthread)
endif(SOCK)

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
