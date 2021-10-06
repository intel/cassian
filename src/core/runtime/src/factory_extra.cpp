/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/runtime.hpp>
#include <memory>

namespace cassian {

std::unique_ptr<Runtime> create_runtime_extra(const std::string & /*name*/) {
  return nullptr;
}

} // namespace cassian
