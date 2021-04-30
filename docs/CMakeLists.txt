#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

find_package(Doxygen REQUIRED)

set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
set(DOXYGEN_EXCLUDE_PATTERNS "*/src/*/src/*" "*/src/test_suites/*")
set(DOXYGEN_EXCLUDE_SYMBOLS "detail")
set(DOXYGEN_WARN_AS_ERROR YES)

doxygen_add_docs(
  docs "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/README.md"
  COMMENT "Generate documentation")
