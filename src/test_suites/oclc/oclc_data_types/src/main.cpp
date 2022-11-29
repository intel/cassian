/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/test_harness/test_config.hpp>
#include <cassian/utility/version.hpp>
#include <test_config.hpp>

int main(int argc, char *argv[]) {
  cassian::print_version();

  cassian::CommandLineParser parser;
  cassian::add_harness_arguments(&parser);
  parser.parse(&argc, argv);

  const TestConfig config(parser);
  set_test_config(config);

  int result = Catch::Session().run(argc, argv);
  return result;
}
