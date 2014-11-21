project(xenmm)
cmake_minimum_required(VERSION 2.8)

option(ONLY_CLIENT "Build Only Client, useful on domain" Off)
option(ONLY_DOCUMENT "[Not Used]" Off)
set(TAX_RATE "AUTO" CACHE STRING "set TAX_RATE")
option(SOCK Off)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR})
find_package(Xen REQUIRED)
option(WITH_LIBXL "Use libxenlight library" ${XL_FOUND})
if(WITH_LIBXL AND NOT XL_FOUND)
   message(FATAL_ERROR "Required xenlight not found")
endif()

set(ENABLE_SOCK ${SOCK})
if(TAX_RATE STREQUAL "AUTO")
   set(AUTO_TAX_RATE True)
   message(STATUS "Setting Tax rate: AUTO")
else()
   message(STATUS "Setting Tax rate: ${TAX_RATE}")
endif()

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(script)
add_subdirectory(src)
add_subdirectory(test)

option(UNIT "Build unit test" Off)
if(UNIT)
    add_subdirectory(unit)
endif()

option(DEB "Build deb package" Off)
option(RPM "Build rpm paackge" Off)

if(DEB)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "xiehuc")
    #set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "xenstore-utils,daemon")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "the xen memory manage utils")
    set(CPACK_PACKAGE_DESCRIPTION "the xen memory manage utils")
    include(CPack)
endif(DEB)

if(RPM)
    set(CPACK_GENERATOR "RPM")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "xiehuc")
    #set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "xenstore-utils,daemon")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "the xen memory manage utils")
    set(CPACK_PACKAGE_DESCRIPTION "the xen memory manage utils")
    include(CPack)
endif(RPM)

#add_subdirectory(notebook)
