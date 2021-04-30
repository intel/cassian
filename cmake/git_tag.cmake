#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --tags --exact-match
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE CASSIAN_GIT_TAG
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_VARIABLE CASSIAN_GIT_REVISION ERROR_QUIET
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
endif()
