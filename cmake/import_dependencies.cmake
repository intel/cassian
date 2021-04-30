#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

set(Catch2_DIR "${CMAKE_SOURCE_DIR}/third_party/catch2/lib/cmake/Catch2")
find_package(Catch2 REQUIRED)

option(BUILD_OCL "Build OpenCL runtime" ON)
if(BUILD_OCL)
  set(OCL_ROOT "${CMAKE_SOURCE_DIR}/third_party/opencl")
  find_path(
    OpenCL_INCLUDE_DIR
    NAMES CL/cl_icd.h
    PATHS ${OCL_ROOT}
    PATH_SUFFIXES "include")

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    OpenCL
    REQUIRED_VARS OpenCL_INCLUDE_DIR
    HANDLE_COMPONENTS)
  mark_as_advanced(OpenCL_INCLUDE_DIR)

  if(OpenCL_FOUND)
    list(APPEND OpenCL_INCLUDE_DIRS ${OpenCL_INCLUDE_DIR})
  endif()
endif()

option(BUILD_L0 "Build Level Zero runtime" ON)
if(BUILD_L0)
  set(L0_ROOT "${CMAKE_SOURCE_DIR}/third_party/level_zero")
  find_path(
    LevelZero_INCLUDE_DIR
    NAMES ze_api.h
    PATHS ${L0_ROOT}
    PATH_SUFFIXES "include" "include/level_zero")

  include(FindPackageHandleStandardArgs)
  find_package_handle_standard_args(
    LevelZero REQUIRED_VARS LevelZero_INCLUDE_DIR HANDLE_COMPONENTS)
  mark_as_advanced(LevelZero_INCLUDE_DIR)

  if(LevelZero_FOUND)
    list(APPEND LevelZero_INCLUDE_DIRS ${LevelZero_INCLUDE_DIR})
    if(OpenCL_FOUND)
      list(APPEND LevelZero_INCLUDE_DIRS ${OpenCL_INCLUDE_DIRS})
    endif()
  endif()
endif()
