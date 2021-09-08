/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <memory>
#include <string>

#include <cassian/cli/cli.hpp>
#include <cassian/main/config.hpp>
#include <cassian/runtime/factory.hpp>
#include <cassian/runtime/runtime.hpp>

namespace ca = cassian;

ca::test::Config::Config(const ca::CommandLineParser &parser) {
  runtime_ = ca::create_runtime(parser.get<std::string>("--runtime"));
  runtime_->initialize();

  program_type_ = parser.get<std::string>("--program-type");
}

ca::Runtime *ca::test::Config::runtime() const { return runtime_.get(); }
std::string ca::test::Config::program_type() const { return program_type_; }

const ca::test::Config *config = nullptr;
const ca::test::Config &ca::test::get_config() { return *config; }
void ca::test::set_config(const Config &c) { config = &c; }
