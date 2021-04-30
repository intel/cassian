/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#define CATCH_CONFIG_RUNNER
#include <cassian/utility/version.hpp>
#include <catch2/catch.hpp>

int main(int argc, char *argv[]) {
  cassian::print_version();
  return Catch::Session().run(argc, argv);
}
