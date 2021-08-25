/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/openclc_types.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>
#include <catch2/catch.hpp>
#include <common.hpp>
#include <cstddef>
#include <random>
#include <string>
#include <test_config.hpp>
#include <vector>

namespace ca = cassian;

namespace {

struct TestCase {
  MemoryFlag memory_flag = MemoryFlag::global;
  MemoryOrder memory_order = MemoryOrder::relaxed;
  MemoryScope memory_scope = MemoryScope::device;

  int global_work_size = 0;
  int local_work_size = 0;

  ca::Runtime *runtime = nullptr;
  std::string program_type;
};

std::string get_kernel_path() {
  return "kernels/oclc_atomics/atomic_work_item_fence.cl";
}

std::string memory_flag_build_option(const MemoryFlag memory_flag) {
  return std::string(" -D MEMORY_FLAG=") + to_string(memory_flag);
}

template <typename TEST_TYPE>
std::string get_build_options(const MemoryFlag memory_flag,
                              const MemoryOrder memory_order,
                              const MemoryScope memory_scope) {
  std::string build_options = " -cl-std=CL3.0" +
                              memory_flag_build_option(memory_flag) +
                              memory_order_build_option(memory_order) +
                              memory_scope_build_option(memory_scope);
  return build_options;
}

template <typename TEST_CASE_TYPE> void run_test(TEST_CASE_TYPE test_case) {
  const std::string kernel_path = get_kernel_path();
  const std::string build_options = get_build_options<TEST_CASE_TYPE>(
      test_case.memory_flag, test_case.memory_order, test_case.memory_scope);
  const ca::Kernel kernel = create_kernel(
      kernel_path, build_options, test_case.runtime, test_case.program_type);

  REQUIRE_NOTHROW(test_case.runtime->run_kernel(
      kernel, test_case.global_work_size, test_case.local_work_size));
  test_case.runtime->release_kernel(kernel);
}

template <typename TEST_CASE_TYPE>
void test_signatures(TEST_CASE_TYPE test_case,
                     const std::vector<MemoryFlag> &memory_flags,
                     const std::vector<MemoryOrder> &memory_orders,
                     const std::vector<MemoryScope> &memory_scopes) {
  for (const auto memory_flag : memory_flags) {
    test_case.memory_flag = memory_flag;
    SECTION(to_string(memory_flag)) {

      for (const auto memory_scope : memory_scopes) {
        test_case.memory_scope = memory_scope;
        SECTION(to_string(memory_scope)) {
          if (memory_flag == MemoryFlag::image &&
              memory_scope != MemoryScope::work_item) {
            continue;
          }
          if (!is_memory_scope_supported(
                  test_case.runtime, test_case.program_type, memory_scope)) {
            ca::logging::info()
                << to_string(memory_scope) << " section skipped\n";
            continue;
          }
          for (const auto memory_order : memory_orders) {
            test_case.memory_order = memory_order;
            SECTION(to_string(memory_order)) {
              if (memory_flag == MemoryFlag::image &&
                  memory_order != MemoryOrder::acq_rel) {
                continue;
              }
              if (!is_memory_order_supported(test_case.runtime,
                                             test_case.program_type,
                                             memory_order)) {
                ca::logging::info()
                    << to_string(memory_order) << " section skipped\n";
                continue;
              }
              run_test(test_case);
            }
          }
        }
      }
    }
  }
}

TEST_CASE("atomic_work_item_fence_signatures") {
  const TestConfig &config = get_test_config();

  auto test_case = create_test_case<TestCase>(config);

  test_signatures(test_case, memory_flags_all, memory_orders_all,
                  {MemoryScope::work_group, MemoryScope::device,
                   MemoryScope::all_svm_devices});
}

} // namespace