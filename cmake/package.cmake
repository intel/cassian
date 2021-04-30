#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

set(CPACK_PACKAGE_VENDOR "Intel")
set(CPACK_PACKAGE_CONTACT "Intel Corporation")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Conformance test suite for compute kernel languages")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_OUTPUT_FILE_PREFIX "packages")
include(InstallRequiredSystemLibraries)
include(CPack)
