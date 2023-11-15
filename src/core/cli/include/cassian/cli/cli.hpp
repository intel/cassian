/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_CLI_CLI_HPP
#define CASSIAN_CLI_CLI_HPP

#include <string>
#include <unordered_map>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Class for parsing command line arguments.
 */
class CommandLineParser {
public:
  /**
   * Add an argument to a parser.
   *
   * @param[in] name argument name.
   * @param[in] default_value default argument value.
   */
  void add_argument(const std::string &name,
                    const std::string &default_value = std::string());

  /**
   * Parse command line.
   *
   * @param[in] argc argument count.
   * @param[in] argv argument vector.
   */
  void parse(int *argc, char **argv);

  /**
   * Check if list was requested.
   */
  bool list_requested() const;

  /**
   * Returned value of an argument.
   *
   * @tparam T the type of returned argument value.
   * @param[in] name argument name.
   * @returns argument value.
   * @note T must define a constructor accepting std::string.
   */
  template <typename T> T get(const std::string &name) const;

private:
  std::unordered_map<std::string, std::string> arguments_;

  bool list_requested_ = false;
};

template <typename T> T CommandLineParser::get(const std::string &name) const {
  return T(arguments_.at(name));
}

} // namespace cassian
#endif
