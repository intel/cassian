#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_executable(test_fp_types src/main.cpp src/half.cpp src/common.hpp)

target_link_libraries(test_fp_types PRIVATE Catch2::Catch2 cassian::fp_types
                                            cassian::utility)

set_target_properties(test_fp_types PROPERTIES FOLDER tests/core)
cassian_install_target(test_fp_types)

add_test(NAME test_fp_types COMMAND test_fp_types)
