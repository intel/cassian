#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_executable(test_runtime src/main.cpp src/runtime.cpp src/feature.cpp
                            src/openclc_types.cpp)

target_link_libraries(test_runtime PRIVATE Catch2::Catch2 cassian::runtime
                                           cassian::vector cassian::utility)

set_target_properties(test_runtime PROPERTIES FOLDER tests/core)
cassian_install_target(test_runtime)

add_test(NAME test_runtime COMMAND test_runtime)
