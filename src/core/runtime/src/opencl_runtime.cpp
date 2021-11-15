/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

#include <CL/cl.h>
#include <CL/cl_ext.h>
#include <CL/cl_ext_intel.h>
#include <CL/cl_platform.h>

#include <cassian/logging/logging.hpp>
#include <cassian/offline_compiler/offline_compiler.hpp>
#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/opencl_utils.hpp>
#include <cassian/runtime/program_descriptor.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/utility.hpp>

#include <opencl_runtime.hpp>
#include <opencl_wrapper.hpp>

namespace cassian {
OpenCLRuntime::~OpenCLRuntime() {
  if (queue_ != nullptr) {
    wrapper_.clReleaseCommandQueue(queue_);
  }
  if (context_ != nullptr) {
    wrapper_.clReleaseContext(context_);
  }
}
void OpenCLRuntime::initialize() {
  logging::info() << "Runtime: " << name() << '\n';

  cl_int result = CL_SUCCESS;

  cl_uint number_of_platforms = 0;
  result = wrapper_.clGetPlatformIDs(0, nullptr, &number_of_platforms);
  if (result != CL_SUCCESS) {
    throw RuntimeException(
        "Failed to get number of available OpenCL platforms");
  }

  std::vector<cl_platform_id> platforms(number_of_platforms);
  result =
      wrapper_.clGetPlatformIDs(number_of_platforms, platforms.data(), nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get OpenCL platform ids");
  }

  const cl_platform_id platform = platforms[0];

  cl_uint number_of_devices = 0;
  result = wrapper_.clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 0, nullptr,
                                   &number_of_devices);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get number of available OpenCL devices");
  }

  std::vector<cl_device_id> devices(number_of_devices);
  result = wrapper_.clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL,
                                   number_of_devices, devices.data(), nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get OpenCL device ids");
  }

  device_ = devices[0];

  context_ =
      wrapper_.clCreateContext(nullptr, 1, &device_, nullptr, nullptr, &result);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL context");
  }

  queue_ = wrapper_.clCreateCommandQueueWithProperties(context_, device_,
                                                       nullptr, &result);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL command queue");
  }

  std::string extension_string;

  size_t size = 0;
  result = wrapper_.clGetDeviceInfo(device_, CL_DEVICE_EXTENSIONS, 0, nullptr,
                                    &size);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get size of OpenCL device info");
  }

  if (size > 0) {
    extension_string = std::string(size - 1, 0);
    result = wrapper_.clGetDeviceInfo(device_, CL_DEVICE_EXTENSIONS, size,
                                      &extension_string[0], nullptr);
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to get OpenCL device info");
    }
  }

  std::istringstream iss(extension_string);
  extensions_.insert(std::istream_iterator<std::string>(iss),
                     std::istream_iterator<std::string>());
}

Buffer OpenCLRuntime::create_buffer(const size_t size, AccessQualifier access) {
  cl_int result = CL_SUCCESS;
  const cl_mem_flags flags = append_access_qualifier_flags(0, access);
  cl_mem buffer =
      wrapper_.clCreateBuffer(context_, flags, size, nullptr, &result);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL buffer");
  }

  auto id = reinterpret_cast<std::uintptr_t>(buffer);
  buffers_[id] = buffer;

  return {id, size};
}

Image OpenCLRuntime::create_image(const ImageDimensions dim,
                                  const ImageType type,
                                  const ImageFormat format,
                                  const ImageChannelOrder order,
                                  const AccessQualifier access) {
  cl_int result = CL_SUCCESS;
  const cl_image_format image_format = cl_create_image_format(format, order);
  cl_image_desc desc = {};
  desc.image_type = cl_get_image_type(type);
  desc.image_width = dim.width;
  desc.image_height = dim.height;
  desc.image_depth = dim.depth;
  desc.image_array_size = 0;
  desc.image_row_pitch = 0;
  desc.image_slice_pitch = 0;
  desc.num_mip_levels = 0;
  desc.num_samples = 0;
  const cl_mem_flags flags = append_access_qualifier_flags(0, access);
  cl_mem image = wrapper_.clCreateImage(context_, flags, &image_format, &desc,
                                        nullptr, &result);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL image");
  }
  auto id = reinterpret_cast<std::uintptr_t>(image);
  images_[id] = image;
  return {id, dim};
}

Sampler OpenCLRuntime::create_sampler(SamplerCoordinates coordinates,
                                      SamplerAddressingMode address_mode,
                                      SamplerFilterMode filter_mode) {
  cl_int result = CL_SUCCESS;

  cl_bool coord = CL_TRUE;
  cl_addressing_mode addr = CL_ADDRESS_CLAMP;
  cl_filter_mode filter = CL_FILTER_NEAREST;

  switch (coordinates) {
  case SamplerCoordinates::unnormalized:
    coord = CL_FALSE;
    break;
  case SamplerCoordinates::normalized:
    coord = CL_TRUE;
    break;
  }

  switch (address_mode) {
  case SamplerAddressingMode::none:
    addr = CL_ADDRESS_NONE;
    break;
  case SamplerAddressingMode::clamp_to_edge:
    addr = CL_ADDRESS_CLAMP_TO_EDGE;
    break;
  case SamplerAddressingMode::clamp:
    addr = CL_ADDRESS_CLAMP;
    break;
  case SamplerAddressingMode::repeat:
    addr = CL_ADDRESS_REPEAT;
    break;
  case SamplerAddressingMode::mirror:
    addr = CL_ADDRESS_MIRRORED_REPEAT;
    break;
  }

  switch (filter_mode) {
  case SamplerFilterMode::nearest:
    filter = CL_FILTER_NEAREST;
    break;
  case SamplerFilterMode::linear:
    filter = CL_FILTER_LINEAR;
    break;
  }

  cl_sampler sampler =
      wrapper_.clCreateSampler(context_, coord, addr, filter, &result);

  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL sampler");
  }

  auto id = reinterpret_cast<std::uintptr_t>(sampler);
  samplers_[id] = sampler;
  return {id};
}

void OpenCLRuntime::read_buffer(const Buffer &buffer, void *data) {
  cl_mem b = buffers_.at(buffer.id);
  cl_int result = wrapper_.clEnqueueReadBuffer(queue_, b, 1, 0, buffer.size,
                                               data, 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to read OpenCL buffer");
  }
}

void OpenCLRuntime::read_image(const Image &image, void *data) {
  cl_mem src_image = images_.at(image.id);
  const size_t region[] = {image.dim.width, image.dim.height, image.dim.depth};
  const size_t origin[] = {0, 0, 0};
  cl_int result = wrapper_.clEnqueueReadImage(
      queue_, src_image, 1, origin, region, 0, 0, data, 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to read OpenCL image");
  }
}

void OpenCLRuntime::write_buffer(const Buffer &buffer, const void *data) {
  cl_mem b = buffers_.at(buffer.id);
  cl_int result = wrapper_.clEnqueueWriteBuffer(queue_, b, 1, 0, buffer.size,
                                                data, 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to write to OpenCL buffer");
  }
}

void OpenCLRuntime::write_image(const Image &image, const void *data) {
  cl_mem i = images_.at(image.id);
  const size_t region[] = {image.dim.width, image.dim.height, image.dim.depth};
  const size_t origin[] = {0, 0, 0};
  cl_int result = wrapper_.clEnqueueWriteImage(queue_, i, 1, origin, region, 0,
                                               0, data, 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to write to OpenCL image");
  }
}

void OpenCLRuntime::release_buffer(const Buffer &buffer) {
  cl_mem b = buffers_.at(buffer.id);
  buffers_.erase(buffer.id);

  cl_int result = wrapper_.clReleaseMemObject(b);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to release OpenCL buffer");
  }
}

void OpenCLRuntime::release_image(const Image &image) {
  cl_mem i = images_.at(image.id);
  images_.erase(image.id);
  cl_int result = wrapper_.clReleaseMemObject(i);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to release OpenCL image");
  }
}

void OpenCLRuntime::release_sampler(const Sampler &sampler) {
  cl_sampler s = samplers_.at(sampler.id);
  samplers_.erase(sampler.id);
  cl_int result = wrapper_.clReleaseSampler(s);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to release OpenCL sampler");
  }
}

Kernel OpenCLRuntime::create_kernel(
    const std::string &kernel_name, const std::string &source,
    const std::string &build_options, const std::string &program_type,
    const std::optional<std::string> &spirv_options) {
  cl_int result = CL_SUCCESS;

  cl_program program = cl_create_program(source, build_options, program_type);

  const char *options = build_options.c_str();
  if (program_type == "spirv") {
    if (spirv_options.has_value()) {
      options = spirv_options->c_str();
    } else if (build_options.find("-cmc") == 0) {
      options = "-vc-codegen";
    }
  }

  result =
      wrapper_.clBuildProgram(program, 1, &device_, options, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    const auto build_log = cl_get_program_build_info(program);
    logging::error() << "Build log:\n" << build_log << '\n';
    throw RuntimeException("Failed to build OpenCL program");
  }

  cl_kernel kernel =
      wrapper_.clCreateKernel(program, kernel_name.c_str(), &result);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL kernel");
  }

  result = wrapper_.clReleaseProgram(program);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to release OpenCL program");
  }

  auto id = reinterpret_cast<std::uintptr_t>(kernel);
  kernels_[id] = kernel;

  return Kernel(id);
}

Kernel OpenCLRuntime::create_kernel_from_multiple_programs(
    const std::string &kernel_name,
    const std::vector<ProgramDescriptor> &program_descriptors,
    const std::string &linker_options) {
  cl_int result = CL_SUCCESS;
  std::vector<cl_program> compiled_programs;
  cl_program program = nullptr;

  for (const auto &program_desc : program_descriptors) {
    const auto &compiler_options = program_desc.compiler_options;
    program = cl_create_program(program_desc.source, compiler_options,
                                program_desc.program_type);
    result = wrapper_.clCompileProgram(program, 1, &device_,
                                       compiler_options.c_str(), 0, nullptr,
                                       nullptr, nullptr, nullptr);
    if (result != CL_SUCCESS) {
      const auto build_log = cl_get_program_build_info(program);
      logging::error() << "Build log:\n" << build_log << '\n';
      throw RuntimeException("Failed to compile OpenCL program");
    }
    compiled_programs.push_back(program);
  }
  program = wrapper_.clLinkProgram(
      context_, 1, &device_, linker_options.c_str(), compiled_programs.size(),
      compiled_programs.data(), nullptr, nullptr, &result);
  if (result != CL_SUCCESS) {
    const auto build_log = cl_get_program_build_info(program);
    logging::error() << "Build log:\n" << build_log << '\n';
    throw RuntimeException("Failed to link OpenCL program");
  }
  cl_kernel kernel =
      wrapper_.clCreateKernel(program, kernel_name.c_str(), &result);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL kernel");
  }
  result = wrapper_.clReleaseProgram(program);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to release OpenCL program");
  }
  for (const auto &prog : compiled_programs) {
    result = wrapper_.clReleaseProgram(prog);
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to release OpenCL program");
    }
  }
  auto id = reinterpret_cast<std::uintptr_t>(kernel);
  kernels_[id] = kernel;

  return Kernel(id);
}

void OpenCLRuntime::set_kernel_argument(const Kernel &kernel,
                                        const int argument_index,
                                        const Buffer &buffer) {
  cl_mem b = buffers_.at(buffer.id);
  set_kernel_argument(kernel, argument_index, sizeof(b), &b);
}

void OpenCLRuntime::set_kernel_argument(const Kernel &kernel,
                                        const int argument_index,
                                        const Image &image) {
  cl_mem i = images_.at(image.id);
  set_kernel_argument(kernel, argument_index, sizeof(i), &i);
}

void OpenCLRuntime::set_kernel_argument(const Kernel &kernel,
                                        const int argument_index,
                                        const Sampler &sampler) {
  cl_sampler s = samplers_.at(sampler.id);
  set_kernel_argument(kernel, argument_index, sizeof(s), &s);
}

void OpenCLRuntime::set_kernel_argument(const Kernel &kernel,
                                        const int argument_index,
                                        const size_t argument_size,
                                        const void *argument) {
  cl_kernel k = kernels_.at(kernel.id);
  cl_int result =
      wrapper_.clSetKernelArg(k, argument_index, argument_size, argument);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to set OpenCL kernel argument");
  }
}

void OpenCLRuntime::run_kernel_common(
    const Kernel &kernel, const std::array<size_t, 3> global_work_size,
    const std::array<size_t, 3> *local_work_size) {
  cl_kernel k = kernels_.at(kernel.id);
  cl_uint work_dim = (global_work_size[1] > 1U) ? 2 : 1U;
  work_dim = (global_work_size[2] > 1U) ? 3 : work_dim;

  std::array<size_t, 3> global_work_offset = {0, 0, 0};
  std::array<size_t, 3> local_ws = {1, 1, 1};
  if (local_work_size != nullptr) {
    local_ws = *local_work_size;
  } else {
    const std::array<size_t, 3> max_limits = {
        static_cast<size_t>(
            get_device_property(DeviceProperty::max_group_size_x)),
        static_cast<size_t>(
            get_device_property(DeviceProperty::max_group_size_y)),
        static_cast<size_t>(
            get_device_property(DeviceProperty::max_group_size_z))};
    const auto max_common_size = static_cast<size_t>(
        get_device_property(DeviceProperty::max_total_group_size));
    local_ws =
        get_max_local_work_size(global_work_size, max_limits, max_common_size);
  }
  logging::debug() << "Running kernel with global_work_size = "
                   << to_string(global_work_size)
                   << " and local_work_size = " << to_string(local_ws) << '\n';
  cl_int result = wrapper_.clEnqueueNDRangeKernel(
      queue_, k, work_dim, global_work_offset.data(), global_work_size.data(),
      local_ws.data(), 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to enqueue OpenCL ND range kernel");
  }

  result = wrapper_.clFinish(queue_);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to finish OpenCL queue");
  }
}

void OpenCLRuntime::release_kernel(const Kernel &kernel) {
  cl_kernel k = kernels_.at(kernel.id);
  kernels_.erase(kernel.id);

  cl_int result = wrapper_.clReleaseKernel(k);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to release OpenCL kernel");
  }
}

bool OpenCLRuntime::is_feature_supported(const Feature feature) const {
  switch (feature) {
  case Feature::fp16:
    return extensions_.count("cl_khr_fp16") != 0U;
  case Feature::fp64:
    return extensions_.count("cl_khr_fp64") != 0U;
  case Feature::read_write_images: {
    cl_uint read_write_image = 0;
    auto result = wrapper_.clGetDeviceInfo(
        device_, CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS, sizeof(read_write_image),
        &read_write_image, nullptr);
    if (result != CL_SUCCESS) {
        throw RuntimeException("Failed to get OpenCL device info");
    }
    return read_write_image != 0;
  }
  case Feature::image:
  case Feature::image2d:
    return get_device_property(DeviceProperty::image) !=
           static_cast<int>(CL_FALSE);
  case Feature::sampling:
    return get_device_property(DeviceProperty::max_num_samplers) != 0;
  case Feature::int64_atomics:
    return extensions_.count("cl_khr_int64_base_atomics") != 0U &&
           extensions_.count("cl_khr_int64_extended_atomics") != 0U;
  default:
    return false;
  }
}

int OpenCLRuntime::get_device_property(const DeviceProperty property) const {
  switch (property) {
  case DeviceProperty::max_group_size_x:
    return static_cast<int>(cl_get_device_property_at_index<size_t>(
        device_, CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, 1));
  case DeviceProperty::max_group_size_y:
    return static_cast<int>(cl_get_device_property_at_index<size_t>(
        device_, CL_DEVICE_MAX_WORK_ITEM_SIZES, 1, 1));
  case DeviceProperty::max_group_size_z:
    return static_cast<int>(cl_get_device_property_at_index<size_t>(
        device_, CL_DEVICE_MAX_WORK_ITEM_SIZES, 2, 1));
  case DeviceProperty::max_total_group_size:
    return static_cast<int>(cl_get_device_property_at_index<size_t>(
        device_, CL_DEVICE_MAX_WORK_GROUP_SIZE, 0));
  case DeviceProperty::max_num_samplers:
    return static_cast<int>(cl_get_device_property_at_index<cl_uint>(
        device_, CL_DEVICE_MAX_SAMPLERS, 0));
  case DeviceProperty::image:
  case DeviceProperty::image2d:
    return static_cast<int>(cl_get_device_property_at_index<cl_bool>(
        device_, CL_DEVICE_IMAGE_SUPPORT, 0));
  case DeviceProperty::max_local_memory_size:
    return static_cast<int>(cl_get_device_property_at_index<cl_ulong>(
        device_, CL_DEVICE_LOCAL_MEM_SIZE, 0));
  case DeviceProperty::device_id:
    // No standard way to detect device ID. Using Intel extension
#if defined(cl_intel_device_attribute_query)
    if (extensions_.count("cl_intel_device_attribute_query") != 0U) {
      return static_cast<int>(cl_get_device_property_at_index<cl_uint>(
          device_, CL_DEVICE_ID_INTEL, 0));
    }
#endif // defined(cl_intel_device_attribute_query)
    return 0;
  case DeviceProperty::simd_width: {
    // OpenCL has no way to detect SIMD width. Trying Intel extension
    if (extensions_.count("cl_intel_required_subgroup_size") != 0U) {
      auto sizes = cl_get_device_property<size_t>(
          device_, CL_DEVICE_SUB_GROUP_SIZES_INTEL);
      return static_cast<int>(
          *std::min_element(std::begin(sizes), std::end(sizes)));
    }
    return 0;
  }
  default:
    throw RuntimeException("Failed to find device property");
  }
}

std::string OpenCLRuntime::name() const { return "OCL"; }

std::string
OpenCLRuntime::cl_get_program_build_info(const cl_program &program) const {
  size_t log_size = 0;
  cl_int result = wrapper_.clGetProgramBuildInfo(
      program, device_, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get OpenCL build log size");
  }
  std::vector<char> log_vector(log_size);
  result = wrapper_.clGetProgramBuildInfo(
      program, device_, CL_PROGRAM_BUILD_LOG, log_vector.size(),
      log_vector.data(), nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get OpenCL build log");
  }
  std::string log(log_vector.begin(), log_vector.end());
  return log;
}

cl_program OpenCLRuntime::cl_create_program(const std::string &source,
                                            const std::string &compile_options,
                                            const std::string &program_type) {
  cl_program program = nullptr;
  cl_int result = CL_SUCCESS;
  if (program_type == "source") {
    const char *c_source = source.c_str();
    program = wrapper_.clCreateProgramWithSource(context_, 1, &c_source,
                                                 nullptr, &result);
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to create OpenCL program from source");
    }
  } else if (program_type == "spirv") {
    auto device_id = get_device_property(DeviceProperty::device_id);

    const std::vector<uint8_t> spv =
        generate_spirv_from_source(device_id, source, compile_options);

    program = wrapper_.clCreateProgramWithIL(
        context_, spv.data(), sizeof(uint8_t) * spv.size(), &result);
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to create OpenCL program from IL");
    }
  } else {
    throw RuntimeException("Invalid program type: " + program_type);
  }
  return program;
}
} // namespace cassian
