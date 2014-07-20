project(xenmm)
cmake_minimum_required(VERSION 2.8)

option(ONLY_CLIENT "Build Only Client, useful on domain" Off)
option(ONLY_DOCUMENT "[Not Used]" Off)

option(SOCK Off)

if( NOT ONLY_DOCUMENT)
add_subdirectory(script)
add_subdirectory(src)
add_subdirectory(test)
endif( NOT ONLY_DOCUMENT)

option(UNIT "Build unit test" Off)
if(UNIT)
    add_subdirectory(unit)
endif(UNIT)

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
