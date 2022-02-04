/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_MOCKS_DUMMY_RUNTIME_HPP
#define CASSIAN_RUNTIME_MOCKS_DUMMY_RUNTIME_HPP

#include <array>
#include <cassian/runtime/access_qualifier.hpp>
#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/program_descriptor.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cstddef>
#include <string>

namespace cassian {

/**
 * Dummy implementation of Runtime interface.
 */
class DummyRuntime : public Runtime {
public:
  void initialize() override;

  Buffer create_buffer(size_t size, AccessQualifier access) override;
  Image create_image(const ImageDimensions dim, const ImageType type,
                     const ImageFormat format, const ImageChannelOrder order,
                     AccessQualifier access) override;
  Sampler create_sampler(SamplerCoordinates coordinates,
                         SamplerAddressingMode address_mode,
                         SamplerFilterMode filter_mode) override;

  void read_buffer(const Buffer &buffer, void *data) override;
  void read_image(const Image &image, void *data) override;
  void write_buffer(const Buffer &buffer, const void *data) override;
  void write_image(const Image &image, const void *data) override;

  void release_buffer(const Buffer &buffer) override;
  void release_image(const Image &image) override;
  void release_sampler(const Sampler &sampler) override;

  Kernel create_kernel(const std::string &kernel_name,
                       const std::string &source,
                       const std::string &build_options,
                       const std::string &program_type,
                       const std::optional<std::string> &spirv_options,
                       bool quiet) override;
  Kernel create_kernel_from_multiple_programs(
      const std::string &kernel_name,
      const std::vector<ProgramDescriptor> &program_descriptors,
      const std::string &linker_options, bool quiet) override;
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
};

} // namespace cassian

#endif
