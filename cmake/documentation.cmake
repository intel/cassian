#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

option(BUILD_DOCS "Build documentation" OFF)
if(BUILD_DOCS)
  add_subdirectory(docs)
endif()
