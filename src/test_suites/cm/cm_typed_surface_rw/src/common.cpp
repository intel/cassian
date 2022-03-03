/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <common.hpp>

std::string dump_cmask(CMask cmask) {
  std::string result;

  if ((cmask & a) != 0) {
    result += "A";
  }
  if ((cmask & b) != 0) {
    result += "B";
  }
  if ((cmask & g) != 0) {
    result += "G";
  }
  if ((cmask & r) != 0) {
    result += "R";
  }

  return result;
}