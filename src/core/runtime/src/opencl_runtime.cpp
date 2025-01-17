/*
 * Copyright (C) 2021-2023 Intel Corporation
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
#include <CL/cl_platform.h>

#include <cassian/logging/logging.hpp>
#include <cassian/offline_compiler/offline_compiler.hpp>
#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/opencl_utils.hpp>
#include <cassian/runtime/program_descriptor.hpp>
#include <cassian/runtime/property_checks.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/utility.hpp>

#include <opencl_runtime.hpp>
#include <opencl_wrapper.hpp>

namespace cassian {
OpenCLRuntime::~OpenCLRuntime() {
  for (cl_command_queue queue : queues_) {
    if (queue != nullptr) {
      wrapper_.clReleaseCommandQueue(queue);
    }
  }
  for (cl_context context : contexts_) {
    if (context != nullptr) {
      wrapper_.clReleaseContext(context);
    }
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

  devices_.push_back(devices[0]);

  contexts_.push_back(wrapper_.clCreateContext(nullptr, 1, &devices_[0],
                                               nullptr, nullptr, &result));
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL context");
  }

  queues_.push_back(wrapper_.clCreateCommandQueueWithProperties(
      contexts_[0], devices_[0], nullptr, &result));
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL command queue");
  }

  std::string extension_string;

  size_t size = 0;
  result = wrapper_.clGetDeviceInfo(devices_[0], CL_DEVICE_EXTENSIONS, 0,
                                    nullptr, &size);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get size of OpenCL device info");
  }

  if (size > 0) {
    extension_string = std::string(size - 1, 0);
    result = wrapper_.clGetDeviceInfo(devices_[0], CL_DEVICE_EXTENSIONS, size,
                                      &extension_string[0], nullptr);
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to get OpenCL device info");
    }
  }

  std::istringstream iss(extension_string);
  extensions_.insert(std::istream_iterator<std::string>(iss),
                     std::istream_iterator<std::string>());
  root_devices_count_ = devices_.size();
}

void OpenCLRuntime::initialize_subdevices() {
  const cl_device_partition_property partition_properties[] = {
      CL_DEVICE_PARTITION_BY_AFFINITY_DOMAIN, CL_DEVICE_AFFINITY_DOMAIN_NUMA,
      0};

  subdevice_offsets_.resize(devices_.size());

  for (int i = 0; i < root_devices_count_; i++) {
    cl_int result = CL_SUCCESS;
    cl_uint num_subdevices = 0;

    result = wrapper_.clCreateSubDevices(devices_[i], partition_properties, 0,
                                         nullptr, &num_subdevices);
    if (result == CL_DEVICE_PARTITION_FAILED) {
      return;
    }
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to get number of OpenCL subdevices");
    }

    subdevice_offsets_[i] = devices_.size();
    devices_.resize(devices_.size() + num_subdevices);
    result = wrapper_.clCreateSubDevices(
        devices_[i], partition_properties, num_subdevices,
        &devices_[subdevice_offsets_[i]], nullptr);
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to create OpenCL subdevices");
    }

    contexts_.resize(contexts_.size() + num_subdevices);
    queues_.resize(queues_.size() + num_subdevices);

    for (int j = 0; j < num_subdevices; j++) {
      contexts_[subdevice_offsets_[i] + j] = wrapper_.clCreateContext(
          nullptr, 1, &devices_[subdevice_offsets_[i] + j], nullptr, nullptr,
          &result);
      if (result != CL_SUCCESS) {
        throw RuntimeException("Failed to create OpenCL context for device "
                               "number: " +
                               (subdevice_offsets_[i] + j));
      }

      queues_[subdevice_offsets_[i] + j] =
          wrapper_.clCreateCommandQueueWithProperties(
              contexts_[subdevice_offsets_[i] + j],
              devices_[subdevice_offsets_[i] + j], nullptr, &result);
      if (result != CL_SUCCESS) {
        throw RuntimeException(
            "Failed to create OpenCL command queue for device "
            "number: " +
            (subdevice_offsets_[i] + j));
      }
    }
  }
}

int OpenCLRuntime::get_subdevice(int root_device, int subdevice) {
  int return_subdevice = -1;
  if (get_subdevice_count(root_device) > 0) {
    return_subdevice = subdevice_offsets_[root_device] + subdevice;
  }
  return return_subdevice;
}

int OpenCLRuntime::get_subdevice_count(int root_device) {
  int subdevice_count = 0;
  if (root_device < root_devices_count_ - 1) {
    subdevice_count =
        subdevice_offsets_[root_device + 1] - subdevice_offsets_[root_device];
  } else if (root_device == root_devices_count_ - 1) {
    subdevice_count =
        static_cast<int>(devices_.size()) - subdevice_offsets_[root_device];
  }
  return subdevice_count;
}

Buffer OpenCLRuntime::create_buffer(int device, const size_t size,
                                    AccessQualifier access) {
  if (device < 0 || device >= devices_.size()) {
    throw RuntimeException("Invalid device");
  }

  cl_int result = CL_SUCCESS;
  const cl_mem_flags flags = append_access_qualifier_flags(0, access);

  cl_mem buffer =
      wrapper_.clCreateBuffer(contexts_[device], flags, size, nullptr, &result);

  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL buffer");
  }

  auto id = reinterpret_cast<std::uintptr_t>(buffer);
  buffers_[id] = buffer;

  return {device, id, size};
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
  desc.mem_object = nullptr; // NOLINT

  cl_mem_flags flags =
      order == ImageChannelOrder::nv12
          ? CL_MEM_HOST_NO_ACCESS | CL_MEM_ACCESS_FLAGS_UNRESTRICTED_INTEL
          : 0;
  flags = append_access_qualifier_flags(flags, access);

  cl_mem image = wrapper_.clCreateImage(contexts_[0], flags, &image_format,
                                        &desc, nullptr, &result);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL image");
  }
  auto id = reinterpret_cast<std::uintptr_t>(image);
  images_[id] = image;
  return {id, dim};
}

Image OpenCLRuntime::get_image_plane(Image image, ImagePlane plane,
                                     AccessQualifier access) {
  cl_int result = CL_SUCCESS;

  cl_image_format image_format = {};
  image_format.image_channel_data_type = CL_UNORM_INT8;

  cl_image_desc desc = {};
  desc.image_type = cl_get_image_type(ImageType::t_2d);
  desc.image_width = 0;
  desc.image_height = 0;
  desc.image_array_size = 0;
  desc.image_row_pitch = 0;
  desc.image_slice_pitch = 0;
  desc.num_mip_levels = 0;
  desc.num_samples = 0;
  desc.mem_object = images_[image.id]; // NOLINT

  auto dim = image.dim;

  switch (plane) {
  case ImagePlane::y:
    image_format.image_channel_order = CL_R;
    desc.image_depth = 0;
    break;
  case ImagePlane::uv:
    image_format.image_channel_order = CL_RG;
    desc.image_depth = 1;
    dim.width /= 2;
    dim.height /= 2;
    break;
  }

  cl_mem_flags flags = append_access_qualifier_flags(0, access);
  cl_mem image_view = wrapper_.clCreateImage(contexts_[0], flags, &image_format,
                                             &desc, nullptr, &result);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL image");
  }
  auto id = reinterpret_cast<std::uintptr_t>(image_view);
  images_[id] = image_view;
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
      wrapper_.clCreateSampler(contexts_[0], coord, addr, filter, &result);

  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to create OpenCL sampler");
  }

  auto id = reinterpret_cast<std::uintptr_t>(sampler);
  samplers_[id] = sampler;
  return {id};
}

void OpenCLRuntime::read_buffer(const Buffer &buffer, void *data) {
  cl_mem b = buffers_.at(buffer.id);
  cl_int result = wrapper_.clEnqueueReadBuffer(
      queues_[buffer.device], b, 1, 0, buffer.size, data, 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to read OpenCL buffer");
  }
}

void OpenCLRuntime::read_image(const Image &image, void *data) {
  cl_mem src_image = images_.at(image.id);
  const size_t region[] = {image.dim.width, image.dim.height, image.dim.depth};
  const size_t origin[] = {0, 0, 0};
  cl_int result =
      wrapper_.clEnqueueReadImage(queues_[0], src_image, 1, origin, region, 0,
                                  0, data, 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to read OpenCL image");
  }
}

void OpenCLRuntime::write_buffer(const Buffer &buffer, const void *data) {
  cl_mem b = buffers_.at(buffer.id);
  cl_int result = wrapper_.clEnqueueWriteBuffer(
      queues_[0], b, 1, 0, buffer.size, data, 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to write to OpenCL buffer");
  }
}

void OpenCLRuntime::write_image(const Image &image, const void *data) {
  cl_mem i = images_.at(image.id);
  const size_t region[] = {image.dim.width, image.dim.height, image.dim.depth};
  const size_t origin[] = {0, 0, 0};
  cl_int result = wrapper_.clEnqueueWriteImage(queues_[0], i, 1, origin, region,
                                               0, 0, data, 0, nullptr, nullptr);
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
    const std::optional<std::string> &spirv_options, bool quiet) {
  cl_int result = CL_SUCCESS;

  cl_program program =
      cl_create_program(source, build_options, program_type, quiet);

  const char *options = build_options.c_str();
  if (program_type == "spirv") {
    if (spirv_options.has_value()) {
      options = spirv_options->c_str();
    } else if (build_options.find("-cmc") == 0) {
      options = "-vc-codegen";
    }
  }

  logging::debug() << "Build options: " << build_options << '\n';
  logging::debug() << "SPIR-V options: " << spirv_options.value_or("") << '\n';
  logging::debug() << "Final build options: " << options << '\n';

  result = wrapper_.clBuildProgram(program, 1, &devices_[0], options, nullptr,
                                   nullptr);
  if (result != CL_SUCCESS) {
    if (!quiet) {
      const auto build_log = cl_get_program_build_info(program);
      logging::error() << "Build log:\n" << build_log << '\n';
    }
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
    const std::string &linker_options, bool quiet) {
  cl_int result = CL_SUCCESS;
  std::vector<cl_program> compiled_programs;
  cl_program program = nullptr;

  for (const auto &program_desc : program_descriptors) {
    const auto &compiler_options = program_desc.compiler_options;
    program = cl_create_program(program_desc.source, compiler_options,
                                program_desc.program_type, quiet);
    result = wrapper_.clCompileProgram(program, 1, &devices_[0],
                                       compiler_options.c_str(), 0, nullptr,
                                       nullptr, nullptr, nullptr);
    if (result != CL_SUCCESS) {
      if (!quiet) {
        const auto build_log = cl_get_program_build_info(program);
        logging::error() << "Build log:\n" << build_log << '\n';
      }
      throw RuntimeException("Failed to compile OpenCL program");
    }
    compiled_programs.push_back(program);
  }
  program = wrapper_.clLinkProgram(
      contexts_[0], 1, &devices_[0], linker_options.c_str(),
      compiled_programs.size(), compiled_programs.data(), nullptr, nullptr,
      &result);
  if (result != CL_SUCCESS) {
    if (!quiet) {
      const auto build_log = cl_get_program_build_info(program);
      logging::error() << "Build log:\n" << build_log << '\n';
    }
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
    int device, const Kernel &kernel,
    const std::array<size_t, 3> global_work_size,
    const std::array<size_t, 3> *local_work_size) {
  if (device < 0 || device >= devices_.size()) {
    throw RuntimeException("Invalid device");
  }

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
      queues_[device], k, work_dim, global_work_offset.data(),
      global_work_size.data(), local_ws.data(), 0, nullptr, nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to enqueue OpenCL ND range kernel");
  }

  result = wrapper_.clFinish(queues_[device]);
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
        devices_[0], CL_DEVICE_MAX_READ_WRITE_IMAGE_ARGS,
        sizeof(read_write_image), &read_write_image, nullptr);
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
  case Feature::fp16_atomics_global_add: {
    return (get_device_property(DeviceProperty::fp16_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_ADD_EXT) != 0;
  }
  case Feature::fp16_atomics_global_load_store: {
    return (get_device_property(DeviceProperty::fp16_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_LOAD_STORE_EXT) != 0;
  }
  case Feature::fp16_atomics_global_min_max: {
    return (get_device_property(DeviceProperty::fp16_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_MIN_MAX_EXT) != 0;
  }
  case Feature::fp16_atomics_local_add: {
    return (get_device_property(DeviceProperty::fp16_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_ADD_EXT) != 0;
  }
  case Feature::fp16_atomics_local_load_store: {
    return (get_device_property(DeviceProperty::fp16_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_LOAD_STORE_EXT) != 0;
  }
  case Feature::fp16_atomics_local_min_max: {
    return (get_device_property(DeviceProperty::fp16_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_MIN_MAX_EXT) != 0;
  }
  case Feature::fp32_atomics_global_load_store: {
    return (get_device_property(DeviceProperty::fp32_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_LOAD_STORE_EXT) != 0;
  }
  case Feature::fp32_atomics_local_load_store: {
    return (get_device_property(DeviceProperty::fp32_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_LOAD_STORE_EXT) != 0;
  }
  case Feature::fp32_atomics_global_add: {
    return (get_device_property(DeviceProperty::fp32_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_ADD_EXT) != 0;
  }
  case Feature::fp32_atomics_global_min_max: {
    return (get_device_property(DeviceProperty::fp32_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_MIN_MAX_EXT) != 0;
  }
  case Feature::fp32_atomics_local_add: {
    return (get_device_property(DeviceProperty::fp32_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_ADD_EXT) != 0;
  }
  case Feature::fp32_atomics_local_min_max: {
    return (get_device_property(DeviceProperty::fp32_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_MIN_MAX_EXT) != 0;
  }
  case Feature::fp64_atomics_global_load_store: {
    return (get_device_property(DeviceProperty::fp64_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_LOAD_STORE_EXT) != 0;
  }
  case Feature::fp64_atomics_local_load_store: {
    return (get_device_property(DeviceProperty::fp64_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_LOAD_STORE_EXT) != 0;
  }
  case Feature::fp64_atomics_global_add: {
    return (get_device_property(DeviceProperty::fp64_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_ADD_EXT) != 0;
  }
  case Feature::fp64_atomics_global_min_max: {
    return (get_device_property(DeviceProperty::fp64_atomics_capabilities) &
            CL_DEVICE_GLOBAL_FP_ATOMIC_MIN_MAX_EXT) != 0;
  }
  case Feature::fp64_atomics_local_add: {
    return (get_device_property(DeviceProperty::fp64_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_ADD_EXT) != 0;
  }
  case Feature::fp64_atomics_local_min_max: {
    return (get_device_property(DeviceProperty::fp64_atomics_capabilities) &
            CL_DEVICE_LOCAL_FP_ATOMIC_MIN_MAX_EXT) != 0;
  }
  case Feature::fp32_correctly_rounded_divide_sqrt: {
    return (get_device_property(DeviceProperty::fp32_config) &
            CL_FP_CORRECTLY_ROUNDED_DIVIDE_SQRT) != 0;
  }
  case Feature::intel_required_subgroup_size: {
    return extensions_.count("cl_intel_required_subgroup_size") != 0U;
  }
  case Feature::simd8: {
    if (extensions_.count("cl_intel_required_subgroup_size") != 0U) {
      auto sizes = cl_get_device_property<size_t>(
          devices_[0], CL_DEVICE_SUB_GROUP_SIZES_INTEL);
      if (std::find(std::begin(sizes), std::end(sizes), 8) != std::end(sizes)) {
        return true;
      }
    }
    return false;
  }
  case Feature::simd16: {
    if (extensions_.count("cl_intel_required_subgroup_size") != 0U) {
      auto sizes = cl_get_device_property<size_t>(
          devices_[0], CL_DEVICE_SUB_GROUP_SIZES_INTEL);
      if (std::find(std::begin(sizes), std::end(sizes), 16) !=
          std::end(sizes)) {
        return true;
      }
    }
    return false;
  }
  case Feature::simd32: {
    if (extensions_.count("cl_intel_required_subgroup_size") != 0U) {
      auto sizes = cl_get_device_property<size_t>(
          devices_[0], CL_DEVICE_SUB_GROUP_SIZES_INTEL);
      if (std::find(std::begin(sizes), std::end(sizes), 32) !=
          std::end(sizes)) {
        return true;
      }
    }
    return false;
  }
  case Feature::integer_dp4a: {
    return (get_device_property(DeviceProperty::dot_product_capabilities) &
            CL_DEVICE_INTEGER_DOT_PRODUCT_INPUT_4x8BIT_KHR) != 0;
  }
  case Feature::integer_dp4a_packed: {
    return (get_device_property(DeviceProperty::dot_product_capabilities) &
            CL_DEVICE_INTEGER_DOT_PRODUCT_INPUT_4x8BIT_PACKED_KHR) != 0;
  }
  default:
    return false;
  }
}

int OpenCLRuntime::get_device_property(const DeviceProperty property) const {
  switch (property) {
  case DeviceProperty::max_group_size_x:
    return static_cast<int>(cl_get_device_property_at_index<size_t>(
        devices_[0], CL_DEVICE_MAX_WORK_ITEM_SIZES, 0, 1));
  case DeviceProperty::max_group_size_y:
    return static_cast<int>(cl_get_device_property_at_index<size_t>(
        devices_[0], CL_DEVICE_MAX_WORK_ITEM_SIZES, 1, 1));
  case DeviceProperty::max_group_size_z:
    return static_cast<int>(cl_get_device_property_at_index<size_t>(
        devices_[0], CL_DEVICE_MAX_WORK_ITEM_SIZES, 2, 1));
  case DeviceProperty::max_total_group_size:
    return static_cast<int>(cl_get_device_property_at_index<size_t>(
        devices_[0], CL_DEVICE_MAX_WORK_GROUP_SIZE, 0));
  case DeviceProperty::max_num_samplers:
    return static_cast<int>(cl_get_device_property_at_index<cl_uint>(
        devices_[0], CL_DEVICE_MAX_SAMPLERS, 0));
  case DeviceProperty::image:
  case DeviceProperty::image2d:
    return static_cast<int>(cl_get_device_property_at_index<cl_bool>(
        devices_[0], CL_DEVICE_IMAGE_SUPPORT, 0));
  case DeviceProperty::max_local_memory_size:
    return static_cast<int>(cl_get_device_property_at_index<cl_ulong>(
        devices_[0], CL_DEVICE_LOCAL_MEM_SIZE, 0));
  case DeviceProperty::device_id:
    // No standard way to detect device ID. Using Intel extension
#if defined(cl_intel_device_attribute_query)
    if (extensions_.count("cl_intel_device_attribute_query") != 0U) {
      return static_cast<int>(cl_get_device_property_at_index<cl_uint>(
          devices_[0], CL_DEVICE_ID_INTEL, 0));
    }
#endif // defined(cl_intel_device_attribute_query)
    return 0;
  case DeviceProperty::device_revision:
    return -1;
  case DeviceProperty::ip_version:
    // No standard way to detect device ID. Using Intel extension
#if defined(cl_intel_device_attribute_query)
    if (extensions_.count("cl_intel_device_attribute_query") != 0U) {
      return static_cast<int>(cl_get_device_property_at_index<cl_uint>(
          devices_[0], CL_DEVICE_IP_VERSION_INTEL, 0));
    }
#endif // defined(cl_intel_device_attribute_query)
    return 0;
  case DeviceProperty::simd_width: {
    // OpenCL has no way to detect SIMD width. Trying Intel extension
    if (extensions_.count("cl_intel_required_subgroup_size") != 0U) {
      auto sizes = cl_get_device_property<size_t>(
          devices_[0], CL_DEVICE_SUB_GROUP_SIZES_INTEL);
      return static_cast<int>(
          *std::min_element(std::begin(sizes), std::end(sizes)));
    }
    return 0;
  }
  case DeviceProperty::fp32_atomics_capabilities: {
    const bool test = extensions_.count("cl_ext_float_atomics") != 0U;
    if (extensions_.count("cl_ext_float_atomics") != 0U) {
      return static_cast<int>(
          cl_get_device_property_at_index<cl_device_fp_atomic_capabilities_ext>(
              devices_[0], CL_DEVICE_SINGLE_FP_ATOMIC_CAPABILITIES_EXT, 0));
    }
    return 0;
  }
  case DeviceProperty::fp64_atomics_capabilities: {
    if (extensions_.count("cl_ext_float_atomics") != 0U) {
      return static_cast<int>(
          cl_get_device_property_at_index<cl_device_fp_atomic_capabilities_ext>(
              devices_[0], CL_DEVICE_DOUBLE_FP_ATOMIC_CAPABILITIES_EXT, 0));
    }
    return 0;
  }
  case DeviceProperty::fp16_atomics_capabilities: {
    if (extensions_.count("cl_ext_float_atomics") != 0U) {
      return static_cast<int>(
          cl_get_device_property_at_index<cl_device_fp_atomic_capabilities_ext>(
              devices_[0], CL_DEVICE_HALF_FP_ATOMIC_CAPABILITIES_EXT, 0));
    }
    return 0;
  }
  case DeviceProperty::fp32_config:
    return static_cast<int>(
        cl_get_device_property_at_index<cl_device_fp_config>(
            devices_[0], CL_DEVICE_SINGLE_FP_CONFIG, 0));
  case DeviceProperty::fp64_config:
    return static_cast<int>(
        cl_get_device_property_at_index<cl_device_fp_config>(
            devices_[0], CL_DEVICE_DOUBLE_FP_CONFIG, 0));
  case DeviceProperty::dot_product_capabilities:
    if (extensions_.count("cl_khr_integer_dot_product") != 0U) {
      return static_cast<int>(cl_get_device_property_at_index<
                              cl_device_integer_dot_product_capabilities_khr>(
          devices_[0], CL_DEVICE_INTEGER_DOT_PRODUCT_CAPABILITIES_KHR, 0));
    }
    return 0;
  default:
    throw RuntimeException("Failed to find device property");
  }
}

std::string OpenCLRuntime::name() const { return "OCL"; }

std::string
OpenCLRuntime::cl_get_program_build_info(const cl_program &program) const {
  size_t log_size = 0;
  cl_int result = wrapper_.clGetProgramBuildInfo(
      program, devices_[0], CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get OpenCL build log size");
  }
  std::vector<char> log_vector(log_size);
  result = wrapper_.clGetProgramBuildInfo(
      program, devices_[0], CL_PROGRAM_BUILD_LOG, log_vector.size(),
      log_vector.data(), nullptr);
  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to get OpenCL build log");
  }
  std::string log(log_vector.begin(), log_vector.end());
  return log;
}

cl_program OpenCLRuntime::cl_create_program(const std::string &source,
                                            const std::string &compile_options,
                                            const std::string &program_type,
                                            bool quiet) {
  cl_program program = nullptr;
  cl_int result = CL_SUCCESS;
  if (program_type == "source") {
    const char *c_source = source.c_str();
    program = wrapper_.clCreateProgramWithSource(contexts_[0], 1, &c_source,
                                                 nullptr, &result);
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to create OpenCL program from source");
    }
  } else if (program_type == "spirv") {
    static const auto ip_version =
        get_device_property(DeviceProperty::ip_version);

    const std::vector<uint8_t> spv =
        generate_spirv_from_source(ip_version, source, compile_options, quiet);

    program = wrapper_.clCreateProgramWithIL(
        contexts_[0], spv.data(), sizeof(uint8_t) * spv.size(), &result);
    if (result != CL_SUCCESS) {
      throw RuntimeException("Failed to create OpenCL program from IL");
    }
  } else {
    throw RuntimeException("Invalid program type: " + program_type);
  }
  return program;
}

std::vector<uint8_t> OpenCLRuntime::create_program_and_get_native_binary(
    const std::string &source, const std::string &build_options,
    const std::string &program_type,
    const std::optional<std::string> &spirv_options, bool quiet) {
  cl_int result = CL_SUCCESS;
  cl_program program =
      cl_create_program(source, build_options, program_type, quiet);
  const char *c_source = source.c_str();
  std::string final_build_options = build_options;

  if (program_type == "spirv") {
    if (spirv_options.has_value()) {
      final_build_options += spirv_options.value();
    } else if (build_options.find("-cmc") == 0) {
      final_build_options = final_build_options + " -vc-codegen";
    }
  }
  const char *options = final_build_options.c_str();
  result = wrapper_.clBuildProgram(program, 1, &devices_[0], options, nullptr,
                                   nullptr);
  if (result != CL_SUCCESS) {
    if (!quiet) {
      const auto build_log = cl_get_program_build_info(program);
      logging::error() << "Build log:\n" << build_log << '\n';
    }
    throw RuntimeException("Failed to build OpenCL program");
  }
  cl_uint num_devices_associated_with_program = 0;
  result = wrapper_.clGetProgramInfo(
      program, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint),
      &num_devices_associated_with_program, nullptr);
  if (CL_SUCCESS != result) {
    throw RuntimeException(
        "Failed to get CL_PROGRAM_NUM_DEVICES from clGetProgramInfo");
  }
  if (num_devices_associated_with_program <= 0) {
    throw RuntimeException("Incorrect number of devices associated with "
                           "program. Should be at least 1");
  }

  std::vector<cl_device_id> devices_associated_with_program(
      num_devices_associated_with_program);
  result = wrapper_.clGetProgramInfo(
      program, CL_PROGRAM_DEVICES,
      sizeof(cl_device_id) * num_devices_associated_with_program,
      devices_associated_with_program.data(), nullptr);

  if (CL_SUCCESS != result) {
    throw RuntimeException(
        "Failed to get CL_PROGRAM_DEVICES from clGetProgramInfo");
  }

  std::vector<cl_device_id>::iterator it;
  it = std::find(devices_associated_with_program.begin(),
                 devices_associated_with_program.end(), devices_[0]);
  if (it == devices_associated_with_program.end()) {
    throw RuntimeException(
        "Given device not associated with compiled program.");
  }
  auto device_index = it - devices_associated_with_program.begin();

  std::vector<size_t> binary_sizes(num_devices_associated_with_program, 0);
  result = wrapper_.clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES,
                                     sizeof(size_t) *
                                         num_devices_associated_with_program,
                                     binary_sizes.data(), nullptr);
  if (CL_SUCCESS != result) {
    throw RuntimeException(
        "Failed to get CL_PROGRAM_BINARY_SIZES from clGetProgramInfo");
  }

  std::vector<std::vector<uint8_t>> all_programs_bytes;
  size_t all_programs_sizes = 0;
  for (auto &each_size : binary_sizes) {
    all_programs_sizes += each_size;
    std::vector<uint8_t> allocate_bytes;
    allocate_bytes.resize(each_size);
    all_programs_bytes.push_back(allocate_bytes);
  }
  result = wrapper_.clGetProgramInfo(program, CL_PROGRAM_BINARIES,
                                     all_programs_sizes,
                                     all_programs_bytes.data(), nullptr);

  if (result != CL_SUCCESS) {
    throw RuntimeException(
        "Failed to get CL_PROGRAM_BINARIES from clGetProgramInfo");
  }
  result = wrapper_.clReleaseProgram(program);

  if (result != CL_SUCCESS) {
    throw RuntimeException("Failed to release OpenCL program");
  }
  return all_programs_bytes[device_index];
}
} // namespace cassian
