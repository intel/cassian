/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_LEVEL_ZERO_RUNTIME_HPP
#define CASSIAN_RUNTIME_LEVEL_ZERO_RUNTIME_HPP

#include <array>
#include <cassian/runtime/access_qualifier.hpp>
#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/program_descriptor.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cstddef>
#include <cstdint>
#include <level_zero_wrapper.hpp>
#include <string>
#include <unordered_map>
#include <ze_api.h>

namespace cassian {

class LevelZeroRuntime : public Runtime {
public:
  ~LevelZeroRuntime();

  void initialize() override;

  Buffer create_buffer(size_t size, AccessQualifier access) override;
  Image create_image(const ImageDimensions dim, const ImageType type,
                     const ImageFormat format, const ImageChannelOrder order,
                     AccessQualifier access) override;
  Sampler create_sampler() override;
  void read_buffer(const Buffer &buffer, void *data) override;
  void read_image(const Image &image, void *data) override;
  void write_buffer(const Buffer &buffer, const void *data) override;
  void write_image(const Image &image, const void *data) override;
  void release_buffer(const Buffer &buffer) override;
  void release_image(const Image &image) override;
  void release_sampler(const Sampler &sampler) override;

  Kernel
  create_kernel(const std::string &kernel_name, const std::string &source,
                const std::string &build_options,
                const std::string &program_type,
                const std::optional<std::string> &spirv_options) override;
  Kernel create_kernel_from_multiple_programs(
      const std::string &kernel_name,
      const std::vector<ProgramDescriptor> &program_descriptors,
      const std::string &linker_options) override;
  void set_kernel_argument(const Kernel &kernel, int argument_index,
                           const Buffer &buffer) override;
  void set_kernel_argument(const Kernel &kernel, int argument_index,
                           const Image &image) override;
  void set_kernel_argument(const Kernel &kernel, int argument_index,
                           const Sampler &sampler) override;
  void release_kernel(const Kernel &kernel) override;

  bool is_feature_supported(Feature feature) const override;
  int get_device_property(DeviceProperty property) const override;

  std::string name() const override;

protected:
  void set_kernel_argument(const Kernel &kernel, int argument_index,
                           size_t argument_size, const void *argument) override;
  void run_kernel_common(const Kernel &kernel,
                         std::array<size_t, 3> global_work_size,
                         const std::array<size_t, 3> *local_work_size) override;

private:
  LevelZeroWrapper wrapper_;

  ze_driver_handle_t driver_ = nullptr;
  ze_device_handle_t device_ = nullptr;
  ze_context_handle_t context_ = nullptr;
  ze_command_queue_handle_t queue_ = nullptr;

  std::unordered_map<std::uintptr_t, void *> buffers_;
  std::unordered_map<std::uintptr_t, ze_image_handle_t> images_;
  std::unordered_map<std::uintptr_t, ze_module_handle_t> modules_;
  std::unordered_map<std::uintptr_t, ze_kernel_handle_t> kernels_;
  std::unordered_map<std::uintptr_t, ze_sampler_handle_t> samplers_;

  uint32_t ze_get_device_id() const;

  std::string ze_get_module_build_log(
      const ze_module_build_log_handle_t &build_log_handle) const;
};

} // namespace cassian
#endif
