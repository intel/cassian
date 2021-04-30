/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "common.hpp"

int suggest_work_size(const std::string &type) {
  const int default_size = 64;
  const int min_size = 4;
  const int max_size = 4096;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}

int suggest_2d_work_size(const std::string &type) {
  const int default_size = 8;
  const int min_size = 4;
  const int max_size = 64;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}

int suggest_3d_work_size(const std::string &type) {
  const int default_size = 4;
  const int min_size = 4;
  const int max_size = 16;
  if (type == "min") {
    return min_size;
  }
  if (type == "max") {
    return max_size;
  }
  return default_size;
}

std::string Catch::work_size_data_to_string(work_size_data const &value) {
  std::stringstream ss;
  ss << "{" << value.work_size_x << ", " << value.work_size_y << ", "
     << value.work_size_z << "}";
  return ss.str();
}

std::vector<size_t> create_divisors_vector(const size_t number) {
  std::vector<size_t> v;
  for (auto i = 2U; i <= number / 2; i++) {
    if (number % i == 0) {
      v.push_back(i);
    }
  }
  return v;
}

std::array<size_t, 3>
get_distributed_work_size(const std::array<size_t, 3> &global_ws,
                          const std::array<size_t, 3> &work_size_limits,
                          const std::array<size_t, 3> &local_ws,
                          const int &work_dim) {
  std::array<size_t, 3> new_local_ws = local_ws;
  constexpr auto epsilon = 4U;
  for (auto i = 1U; i < work_dim; i++) {
    if (local_ws.at(i) <= epsilon) {
      auto &size_to_divide = new_local_ws.at(i - 1);
      auto divisors = create_divisors_vector(size_to_divide);
      auto divisor = divisors[divisors.size() / 2];
      size_to_divide /= divisor;
      auto size_to_distribute = divisor * local_ws.at(i);
      while (global_ws.at(i) % size_to_distribute != 0 &&
                 work_size_limits.at(i) < size_to_distribute ||
             global_ws.at(i) < size_to_distribute) {
        size_to_distribute--;
      }
      new_local_ws.at(i) = size_to_distribute;
    }
  }
  return new_local_ws;
}

std::array<size_t, 3>
get_different_reqd_size(const std::array<size_t, 3> &work_size) {
  auto reqd_size = work_size;
  for (int i = static_cast<int>(work_size.size() - 1); i >= 0; i--) {
    if (reqd_size.at(i) >= 2U) {
      reqd_size.at(i) /= 2U;
      return reqd_size;
    }
  }
  return {1U, 1U, 1U};
}
