#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

add_subdirectory(dummy_library)

add_executable(test_system src/main.cpp src/library.cpp)

target_link_libraries(test_system PRIVATE Catch2::Catch2 cassian::system
                                          cassian::utility)

set_target_properties(test_system PROPERTIES FOLDER tests/core/system)
cassian_install_target(test_system)

add_test(NAME test_system COMMAND test_system)

if(UNIX)
  set_tests_properties(
    test_system PROPERTIES ENVIRONMENT
                           LD_LIBRARY_PATH=$<TARGET_FILE_DIR:dummy_library>)
elseif(WIN32)
  add_custom_command(
    TARGET test_system
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:dummy_library>
            $<TARGET_FILE_DIR:test_system>)
endif()
