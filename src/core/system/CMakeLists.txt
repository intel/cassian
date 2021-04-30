#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

list(APPEND PUBLIC_HEADERS "include/cassian/system/library.hpp"
     "include/cassian/system/factory.hpp")
list(APPEND PRIVATE_HEADERS)
list(APPEND SOURCES "src/library.cpp" "src/factory.cpp")

add_library(system ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::system ALIAS system)

target_include_directories(
  system
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

set_target_properties(system PROPERTIES FOLDER core)
set_target_properties(system PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

if(UNIX)
  list(APPEND LINUX_PUBLIC_HEADERS)
  list(APPEND LINUX_PRIVATE_HEADERS "src/library_linux.hpp")
  list(APPEND LINUX_SOURCES "src/library_linux.cpp")
  target_sources(system PRIVATE ${LINUX_PUBLIC_HEADERS}
                                ${LINUX_PRIVATE_HEADERS} ${LINUX_SOURCES})
  target_link_libraries(system PRIVATE ${CMAKE_DL_LIBS})
elseif(WIN32)
  list(APPEND WINDOWS_PUBLIC_HEADERS)
  list(APPEND WINDOWS_PRIVATE_HEADERS "src/library_windows.hpp")
  list(APPEND WINDOWS_SOURCES "src/library_windows.cpp")
  target_sources(system PRIVATE ${WINDOWS_PUBLIC_HEADERS}
                                ${WINDOWS_PRIVATE_HEADERS} ${WINDOWS_SOURCES})
endif()

cassian_install_target(system)
