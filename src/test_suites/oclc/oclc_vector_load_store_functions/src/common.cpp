/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>
#include <string>

std::string to_string(const AddressSpace address_space) {
  switch (address_space) {
  case AddressSpace::clc_global:
    return "global";
  case AddressSpace::clc_local:
    return "local";
  case AddressSpace::clc_private:
    return "private";
  case AddressSpace::clc_constant:
    return "constant";
  case AddressSpace::clc_generic:
    return "generic";
  default:
    return "unknown";
  }
}

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
