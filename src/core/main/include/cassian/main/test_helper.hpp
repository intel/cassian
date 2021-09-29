/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_MAIN_TEST_HELPER_HPP
#define CASSIAN_MAIN_TEST_HELPER_HPP

#include <cassert>
#include <functional>
#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <cassian/fp_types/type_traits.hpp>

#include "config.hpp"

/**
 * Cassian namespace.
 */
namespace cassian {

class Requirements;

namespace test {
namespace detail {
class Helper {
public:
  using Argument = std::variant<bool, char, signed char, unsigned char, short,
                                unsigned short, int, unsigned int, long,
                                unsigned long, long long, unsigned long long,
                                float, double, cassian::Buffer>;

  using Action = std::function<void(void)>;

  static Helper &instance();

  Helper(const Helper &) = delete;
  Helper(Helper &&) = delete;

  ~Helper() = default;

  Helper &operator=(const Helper &) = delete;
  Helper &operator=(Helper &&) = delete;

  void pass(Argument arg);

  void kernel(const std::string &name, const std::string &source,
              const std::string &build_options,
              const std::optional<std::string> &spirv_options = std::nullopt);

  void execute(std::array<size_t, 3> global_work_size,
               std::array<size_t, 3> local_work_size);

  void cleanup();

  void add_action_after_exec(const Action &action);

  cassian::Buffer create_buffer(size_t size);

  const Config &config;

private:
  Helper();

  cassian::Kernel kernel_;
  std::vector<Argument> arguments_;

  std::vector<cassian::Buffer> buffers_;

  std::vector<Action> after_kernel_exec_;
};
} // namespace detail

void kernel(const std::string &name, const std::string &source,
            const std::string &flags,
            const std::optional<std::string> &spirv_flags = std::nullopt);

void kernel(std::array<size_t, 3> global_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags = std::nullopt);

void kernel(std::array<size_t, 2> global_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags = std::nullopt);

void kernel(std::array<size_t, 1> global_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags = std::nullopt);

void kernel(size_t global_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags = std::nullopt);

void kernel(std::array<size_t, 3> global_work_size,
            std::array<size_t, 3> local_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags = std::nullopt);

void kernel(std::array<size_t, 2> global_work_size,
            std::array<size_t, 2> local_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags = std::nullopt);

void kernel(std::array<size_t, 1> global_work_size,
            std::array<size_t, 1> local_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags = std::nullopt);

template <typename T> void pass(const T arg) {
  static_assert(std::is_arithmetic_v<T> || cassian::is_floating_point_v<T>,
                "Unsupported argument type");

  if constexpr (std::is_arithmetic_v<T>) {
    detail::Helper::instance().pass(arg);
  } else if constexpr (cassian::is_floating_point_v<T>) {
    detail::Helper::instance().pass(arg.decode());
  }
}

template <typename T, typename Allocator>
void input(const std::vector<T, Allocator> &data) {
  auto &h = detail::Helper::instance();
  auto *rt = h.config.runtime();

  auto buffer = h.create_buffer(data.size() * sizeof(T));
  rt->write_buffer_from_vector(buffer, data);
  h.pass(buffer);
}

template <typename T, typename Allocator>
void output(std::vector<T, Allocator> &data, size_t size = 0) {
  auto &h = detail::Helper::instance();
  auto *rt = h.config.runtime();

  auto buffer = h.create_buffer((size ? size : data.size()) * sizeof(T));
  h.pass(buffer);

  h.add_action_after_exec(
      [rt, &data, buffer]() { data = rt->read_buffer_to_vector<T>(buffer); });
}

template <typename T, typename Allocator>
void input_output(std::vector<T, Allocator> &data) {
  auto &h = detail::Helper::instance();
  auto *rt = h.config.runtime();

  auto buffer = h.create_buffer(data.size() * sizeof(T));
  rt->write_buffer_from_vector(buffer, data);
  h.pass(buffer);

  h.add_action_after_exec(
      [rt, &data, buffer]() { data = rt->read_buffer_to_vector<T>(buffer); });
}

bool should_skip_test(const Requirements &requirements);

} // namespace test
} // namespace cassian

#endif
