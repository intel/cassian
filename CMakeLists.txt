#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

cmake_minimum_required(VERSION 3.10)

set(VERSION 0.0.0)
include(cmake/git_tag.cmake)
if(CASSIAN_GIT_TAG)
  set(VERSION ${CASSIAN_GIT_TAG})
endif()

project(cassian VERSION ${VERSION})

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX
        "${CMAKE_BINARY_DIR}/install"
        CACHE PATH "Default install path" FORCE)
  endif()

  include(cmake/compilation_flags.cmake)
endif()

include(cmake/cassian_utils.cmake)
include(cmake/import_dependencies.cmake)

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
  add_subdirectory(tests)
endif()

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(cmake/package.cmake)
  include(cmake/documentation.cmake)
endif()
