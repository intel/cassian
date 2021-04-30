#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

function(cassian_install_target name)
  include(GNUInstallDirs)
  install(
    TARGETS ${name}
    EXPORT ${name}-targets
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cassian/${name}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
  install(
    EXPORT ${name}-targets
    FILE ${name}-targets.cmake
    NAMESPACE cassian::
    DESTINATION lib/cmake/cassian)

  # TODO: Move it to different CMake function
  get_target_property(type ${name} TYPE)
  if(NOT ${type} STREQUAL "INTERFACE_LIBRARY")
    set_target_properties(${name} PROPERTIES OUTPUT_NAME cassian_${name}
                                             VERSION ${PROJECT_VERSION})
  endif()
endfunction()

function(cassian_target_add_kernels name)
  target_sources(${name} PRIVATE ${ARGN})
  source_group("Kernel Files" FILES ${ARGN})
  set_source_files_properties(${ARGN} PROPERTIES HEADER_FILE_ONLY TRUE)

  include(GNUInstallDirs)
  foreach(kernel ${ARGN})
    get_filename_component(kernel_path ${kernel} DIRECTORY)
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
                            "${CMAKE_CURRENT_BINARY_DIR}/${kernel_path}")
    execute_process(
      COMMAND
        ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_SOURCE_DIR}/${kernel}"
        "${CMAKE_CURRENT_BINARY_DIR}/${kernel}")
    install(FILES ${kernel}
            DESTINATION ${CMAKE_INSTALL_DATADIR}/cassian/${kernel_path})
  endforeach()
endfunction()
