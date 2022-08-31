/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/mocks/dummy_runtime.hpp>
#include <cassian/runtime/program_descriptor.hpp>
#include <cassian/runtime/runtime.hpp>

namespace cassian {

void DummyRuntime::initialize() {}

Buffer DummyRuntime::create_buffer(const size_t /*size*/,
                                   AccessQualifier /*access*/) {
  return Buffer();
}

Image DummyRuntime::create_image(const ImageDimensions /*dim*/,
                                 const ImageType /*type*/,
                                 const ImageFormat /*format*/,
                                 const ImageChannelOrder /*order*/,
                                 AccessQualifier /*access*/) {
  return Image();
}

Image DummyRuntime::get_image_plane(Image /*image*/, ImagePlane /*plane*/,
                                    AccessQualifier /*access*/) {
  return Image();
}

Sampler DummyRuntime::create_sampler(SamplerCoordinates /*coordinates*/,
                                     SamplerAddressingMode /*address_mode*/,
                                     SamplerFilterMode /*filter_mode*/) {
  return Sampler();
}

void DummyRuntime::read_buffer(const Buffer & /*buffer*/, void * /*data*/) {}

void DummyRuntime::read_image(const Image & /*image*/, void * /*data*/) {}

void DummyRuntime::write_buffer(const Buffer & /*buffer*/,
                                const void * /*data*/) {}

void DummyRuntime::write_image(const Image & /*image*/, const void * /*data*/) {
}

void DummyRuntime::release_buffer(const Buffer & /*buffer*/) {}

void DummyRuntime::release_image(const Image & /*image*/) {}

void DummyRuntime::release_sampler(const Sampler & /*sampler*/) {}

Kernel DummyRuntime::create_kernel(
    const std::string & /*kernel_name*/, const std::string & /*source*/,
    const std::string & /*build_options*/, const std::string & /*program_type*/,
    const std::optional<std::string> & /*spirv_options*/, bool /*quiet*/) {
  return Kernel();
}

Kernel DummyRuntime::create_kernel_from_multiple_programs(
    const std::string & /*kernel_name*/,
    const std::vector<ProgramDescriptor> & /*program_descriptors*/,
    const std::string & /*linker_options*/, bool /*quiet*/) {
  return Kernel();
}

void DummyRuntime::set_kernel_argument(const Kernel & /*kernel*/,
                                       const int /*argument_index*/,
                                       const Buffer & /*buffer*/) {}

void DummyRuntime::set_kernel_argument(const Kernel & /*kernel*/,
                                       const int /*argument_index*/,
                                       const Image & /*image*/) {}

void DummyRuntime::set_kernel_argument(const Kernel & /*kernel*/,
                                       const int /*argument_index*/,
                                       const Sampler & /*sampler*/) {}

void DummyRuntime::set_kernel_argument(const Kernel & /*kernel*/,
                                       int /*argument_index*/,
                                       size_t /*argument_size*/,
                                       const void * /*argument*/) {}

void DummyRuntime::release_kernel(const Kernel & /*kernel*/) {}

bool DummyRuntime::is_feature_supported(const Feature /*feature*/) const {
  return true;
}

int DummyRuntime::get_device_property(const DeviceProperty /*property*/) const {
  return 0;
}

std::string DummyRuntime::name() const { return "Dummy"; }

void DummyRuntime::run_kernel_common(
    const Kernel & /*kernel*/, std::array<size_t, 3> /*global_work_size*/,
    const std::array<size_t, 3> * /*local_work_size*/) {}

std::vector<uint8_t> DummyRuntime::create_program_and_get_native_binary(
    const std::string & /*source*/, const std::string & /*build_options*/,
    const std::string & /*program_type*/,
    const std::optional<std::string> & /*spirv_options*/, bool /*quiet*/) {
  return std::vector<uint8_t>();
}
} // namespace cassian
