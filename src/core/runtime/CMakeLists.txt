#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

if(NOT DEFINED CASSIAN_RUNTIME_FACTORY_EXTRA)
  set(CASSIAN_RUNTIME_FACTORY_EXTRA "src/factory_extra.cpp")
endif()

list(
  APPEND
  PUBLIC_HEADERS
  "include/cassian/runtime/device_properties.hpp"
  "include/cassian/runtime/runtime.hpp"
  "include/cassian/runtime/factory.hpp"
  "include/cassian/runtime/feature.hpp"
  "include/cassian/runtime/mocks/dummy_runtime.hpp"
  "include/cassian/runtime/mocks/stub_runtime.hpp"
  "include/cassian/runtime/access_qualifier.hpp"
  "include/cassian/runtime/image_properties.hpp"
  "include/cassian/runtime/level_zero_utils.hpp"
  "include/cassian/runtime/opencl_utils.hpp"
  "include/cassian/runtime/openclc_utils.hpp"
  "include/cassian/runtime/openclc_type_tuples.hpp"
  "include/cassian/runtime/openclc_types.hpp"
  "include/cassian/runtime/program_descriptor.hpp"
  "include/cassian/runtime/sampler_properties.hpp")

list(APPEND PRIVATE_HEADERS)
list(
  APPEND
  SOURCES
  "src/runtime.cpp"
  "src/factory.cpp"
  "src/feature.cpp"
  "src/mocks/dummy_runtime.cpp"
  "src/mocks/stub_runtime.cpp"
  "src/image_properties.cpp"
  "src/openclc_utils.cpp"
  ${CASSIAN_RUNTIME_FACTORY_EXTRA})

add_library(runtime ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${SOURCES})
add_library(cassian::runtime ALIAS runtime)

target_include_directories(
  runtime
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

target_link_libraries(
  runtime
  PUBLIC cassian::cli cassian::fp_types cassian::vector cassian::utility
  PRIVATE cassian::offline_compiler cassian::logging cassian::system)

set_target_properties(runtime PROPERTIES FOLDER core)
set_target_properties(runtime PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

if(BUILD_OCL)
  list(APPEND OCL_PUBLIC_HEADERS)
  list(APPEND OCL_PRIVATE_HEADERS "src/opencl_runtime.hpp"
       "src/opencl_wrapper.hpp")
  list(APPEND OCL_SOURCES "src/opencl_runtime.cpp" "src/opencl_wrapper.cpp")
  target_sources(runtime PRIVATE ${OCL_PRIVATE_HEADERS} ${OCL_PRIVATE_HEADERS}
                                 ${OCL_SOURCES})
  target_include_directories(runtime SYSTEM PRIVATE "${OpenCL_INCLUDE_DIRS}")
  target_compile_definitions(runtime PRIVATE BUILD_OCL)
endif()

if(BUILD_L0)
  list(APPEND L0_PUBLIC_HEADERS)
  list(APPEND L0_PRIVATE_HEADERS "src/level_zero_runtime.hpp"
       "src/level_zero_wrapper.hpp")
  list(APPEND L0_SOURCES "src/level_zero_runtime.cpp"
       "src/level_zero_wrapper.cpp")
  target_sources(runtime PRIVATE ${L0_PUBLIC_HEADERS} ${L0_PRIVATE_HEADERS}
                                 ${L0_SOURCES})
  target_include_directories(runtime SYSTEM PRIVATE "${LevelZero_INCLUDE_DIRS}")
  target_compile_definitions(runtime PRIVATE BUILD_L0)
endif()

cassian_install_target(runtime)
