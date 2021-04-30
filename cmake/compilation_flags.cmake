#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

set(CMAKE_CXX_STANDARD 17)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION
                                                 VERSION_LESS 8.0)
  add_definitions(-DCASSIAN_OLD_GNU)
endif()

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

option(USE_ASAN "Use address sanitizer" OFF)
if(USE_ASAN)
  set(CMAKE_C_FLAGS
      "${CMAKE_C_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif()

option(USE_TSAN "Use thread sanitizer" OFF)
if(USE_TSAN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
endif()

option(USE_UBSAN "Use undefined behavior sanitizer" OFF)
if(USE_UBSAN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()

option(USE_LSAN "Use leak sanitizer" OFF)
if(USE_LSAN)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=leak")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
endif()
