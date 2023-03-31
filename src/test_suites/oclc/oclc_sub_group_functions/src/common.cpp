/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>
#include <string>

int suggest_work_size(const std::string &type) {
  const int default_size = 64;
  const int min_size = 1;
  const int max_size = 1024;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}

ca::Kernel create_kernel(const std::string &path,
                         const std::string &kernel_name,
                         const std::string &build_options, ca::Runtime *runtime,
                         const std::string &program_type) {
  const std::string source = ca::load_text_file(ca::get_asset(path));
  return runtime->create_kernel(kernel_name, source, build_options,
                                program_type);
}

std::string function_type(const TestFunctionType &test_function_type) {
  switch (test_function_type) {
  case read_ft:
    return "read";
  case write_ft:
    return "write";
  case read_write_ft:
    return "read_write";
  default:
    return "";
  }
}
std::string create_func_name(TestFunctionType test_function_type,
                             TestExtensionType test_extension_type) {
  std::string func_name = "sub_group";
  switch (test_extension_type) {
  case block:
  case block_image:
    func_name = func_name + "_block_" + function_type(test_function_type);
    printf("fucn_name: %s\n", func_name.c_str());
    break;
  case media_block_image:
    func_name += "_media_block_" + function_type(test_function_type);
    break;
  default:
    func_name += "";
    break;
  }
  return func_name;
}

std::string get_kernel_name(const std::string &name) {
  return "test_kernel_" + name;
}
