# - Try to find Xen Libraries
# Once done this will define
#
#  LWQQ_FOUND - system has LWQQ
#  LWQQ_INCLUDE_DIRS - the LWQQ include directory
#  LWQQ_LIBRARIES - Link these to use LWQQ
#  LWQQ_DEFINITIONS - Compiler switches required for using LWQQ
#
#  Copyright (c) 2008 Tarrisse Laurent <laurent@mbdsys.com>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#

if (XEN_LIBRARIES AND XEN_INCLUDE_DIRS)
   # in cache already
   set(LWQQ_FOUND TRUE)
else ()
   set(LWQQ_DEFINITIONS ${_LWQQCflags})

   find_path(XENSTORE_INCLUDE_DIR
      NAMES xenstore.h
      )
   find_path(XENCTRL_INCLUDE_DIR
      NAMES xenctrl.h
      )
   find_path(XL_INCLUDE_DIR
      NAMES libxl.h
      )

   find_library(XENSTORE_LIBRARY
      NAMES xenstore
      )
   find_library(XENCTRL_LIBRARY
      NAMES xenctrl
      )
   find_library(XL_LIBRARY
      NAMES xenlight
      )

   if(XENSTORE_INCLUDE_DIR AND XENSTORE_LIBRARY)
      set(XENSTORE_FOUND True)
   endif()
   if(XENCTRL_INCLUDE_DIR AND XENCTRL_LIBRARY)
      set(XENCTRL_FOUND True)
   endif()
   if(XL_INCLUDE_DIR AND XL_LIBRARY)
      set(XL_FOUND True)
   endif()

   if(XENSTORE_FOUND)
      message(STATUS "Fond xenstore: ${XENSTORE_LIBRARY}")
   else()
      if(XEN_FIND_REQUIRED)
         message(FATAL_ERROR "Could not find xenstore")
      endif()
   endif()

   if(XENCTRL_FOUND)
      message(STATUS "Fond xenctrl: ${XENCTRL_LIBRARY}")
   else()
      if(XEN_FIND_REQUIRED)
         message(FATAL_ERROR "Could not find xenctrl")
      endif()
   endif()

   if(XL_FOUND)
      message(STATUS "Fond xenlight: ${XENSTORE_LIBRARY}")
   endif()

   mark_as_advanced(XENSTORE_INCLUDE_DIR XENSTORE_LIBRARY XENCTRL_INCLUDE_DIR XENCTRL_LIBRARY XL_INCLUDE_DIR XL_LIBRARY)

endif ()
