/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_MAIN_CONFIG_HPP
#define CASSIAN_MAIN_CONFIG_HPP

#include <memory>
#include <string>

#include <cassian/cli/cli.hpp>
#include <cassian/runtime/runtime.hpp>

namespace cassian {
namespace test {

class Config {
public:
  Config() = default;
  explicit Config(const cassian::CommandLineParser &parser);
  Config(const Config &) = delete;
  Config(Config &&) = delete;
  ~Config() = default;
  Config &operator=(const Config &) = delete;
  Config &operator=(Config &&) = delete;

  cassian::Runtime *runtime() const;
  std::string program_type() const;

private:
  std::unique_ptr<cassian::Runtime> runtime_ = nullptr;
  std::string program_type_ = "";
};

const Config &get_config();
void set_config(const Config &config);

} // namespace test
} // namespace cassian

#endif
