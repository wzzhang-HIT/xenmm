include_directories(
   #${CMAKE_CURRENT_SOURCE_DIR}
   ${PROJECT_BINARY_DIR}
)

add_definitions(-Wall)

if(NOT ONLY_CLIENT)

add_library(util STATIC
    mmstore.c
    mmctrl.c
    mmrecord.c
)

add_executable(mmserver
    mmserver.c
    type.c
    )

if(WITH_LIBXL)
   set(MMSERVER_LIBRARIES ${XL_LIBRARY})
else()
   set(MMSERVER_LIBRARIES ${XENCTRL_LIBRARY})
endif()

if(WITH_SOCK)
   set(MMSERVER_LIBRARIES ${MMSERVER_LIBRARIES} pthread)
endif()

target_link_libraries(mmserver
    util
    m
    ${XENSTORE_LIBRARY}
    ${MMSERVER_LIBRARIES}
    )

install(TARGETS mmserver DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

endif(NOT ONLY_CLIENT)

add_executable(mmclient
   mmclient.c
   mmstore.c
   type.c
)

target_link_libraries(mmclient
   ${XENSTORE_LIBRARY}
)

install(TARGETS mmclient DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
