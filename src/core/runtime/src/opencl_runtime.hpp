/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_OPENCL_RUNTIME_HPP
#define CASSIAN_RUNTIME_OPENCL_RUNTIME_HPP

#include <array>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <CL/cl.h>
#include <CL/cl_platform.h>

#include <cassian/runtime/access_qualifier.hpp>
#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/program_descriptor.hpp>
#include <cassian/runtime/runtime.hpp>

#include "opencl_wrapper.hpp"

namespace cassian {
class OpenCLRuntime : public Runtime {
public:
  void initialize() override;
  void initialize_subdevices() override;
  ~OpenCLRuntime();

  int get_subdevice(int root_device, int subdevice) override;
  int get_subdevice_count(int root_device) override;
  Buffer create_buffer(int device, size_t size,
                       AccessQualifier access) override;
  Image create_image(const ImageDimensions dim, const ImageType type,
                     const ImageFormat format, const ImageChannelOrder order,
                     AccessQualifier access) override;
  Image get_image_plane(Image image, ImagePlane plane,
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
  std::vector<uint8_t> create_program_and_get_native_binary(
      const std::string &source, const std::string &build_options,
      const std::string &program_type,
      const std::optional<std::string> &spirv_options, bool quiet);

protected:
  void set_kernel_argument(const Kernel &kernel, int argument_index,
                           size_t argument_size, const void *argument) override;
  void run_kernel_common(int device, const Kernel &kernel,
                         std::array<size_t, 3> global_work_size,
                         const std::array<size_t, 3> *local_work_size) override;

private:
  OpenCLWrapper wrapper_;

  std::vector<cl_device_id> devices_;
  std::vector<cl_context> contexts_;
  std::vector<cl_command_queue> queues_;

  std::unordered_map<std::uintptr_t, cl_mem> buffers_;
  std::unordered_map<std::uintptr_t, cl_mem> images_;
  std::unordered_map<std::uintptr_t, cl_kernel> kernels_;
  std::unordered_map<std::uintptr_t, cl_sampler> samplers_;

  std::unordered_set<std::string> extensions_;

  std::vector<int> subdevice_offsets_;
  int root_devices_count_ = 0;

  template <class T>
  std::vector<T> cl_get_device_property(cl_device_id device,
                                        cl_device_info param_name) const {
    size_t ret_val_size = 0;
    cl_int result =
        wrapper_.clGetDeviceInfo(device, param_name, 0, nullptr, &ret_val_size);
    if (result != CL_SUCCESS) {
      throw std::runtime_error("Failed to get OpenCL device info.");
    }

    std::vector<T> ret_val(ret_val_size / sizeof(T));
    result = wrapper_.clGetDeviceInfo(device, param_name, ret_val_size,
                                      ret_val.data(), nullptr);
    if (result != CL_SUCCESS) {
      throw std::runtime_error("Failed to get OpenCL device info.");
    }
    return ret_val;
  }

  template <class T>
  T cl_get_device_property_at_index(cl_device_id device,
                                    cl_device_info param_name,
                                    int index) const {
    auto params = cl_get_device_property<T>(device, param_name);
    if (index > params.size() - 1) {
      throw std::runtime_error("Failed to get OpenCL device property");
    }
    return params[index];
  }

  template <class T>
  T cl_get_device_property_at_index(cl_device_id device,
                                    cl_device_info param_name, int index,
                                    T default_val) const {
    auto params = cl_get_device_property<T>(device, param_name);
    if (index > params.size() - 1) {
      return default_val;
    }
    return params[index];
  }

  std::string cl_get_program_build_info(const cl_program &program) const;
  cl_program cl_create_program(const std::string &source,
                               const std::string &compile_options,
                               const std::string &program_type, bool quiet);
};

} // namespace cassian
#endif
