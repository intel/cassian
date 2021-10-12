/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/main/test_helper.hpp>
#include <cassian/test_harness/test_harness.hpp>
#include <cassian/utility/utility.hpp>

namespace cassian::test {

namespace detail {
Helper::Helper() : config(get_config()) {}

Helper &Helper::instance() {
  static Helper helper;
  return helper;
}

void Helper::pass(Helper::Argument arg) { arguments_.push_back(arg); }

void Helper::kernel(const std::string &name, const std::string &source,
                    const std::string &build_options,
                    const std::optional<std::string> &spirv_options) {
  auto *rt = config.runtime();
  auto program_type = config.program_type();

  kernel_ = rt->create_kernel(name, source, build_options, program_type,
                              spirv_options);
}

void Helper::execute(std::array<size_t, 3> global_work_size,
                     std::array<size_t, 3> local_work_size) {
  auto *rt = config.runtime();

  int i = 0;
  for (auto &arg : arguments_) {
    std::visit(
        [this, rt, index = i++](auto value) {
          rt->set_kernel_argument(kernel_, index, value);
        },
        arg);
  }

  rt->run_kernel(kernel_, global_work_size, local_work_size);

  for (const auto &action : after_kernel_exec_) {
    action();
  }
}

void Helper::add_action_after_exec(const Action &action) {
  after_kernel_exec_.push_back(action);
}

void Helper::cleanup() {
  auto *rt = config.runtime();

  for (auto buf : buffers_) {
    rt->release_buffer(buf);
  }

  for (auto img : images_) {
    rt->release_image(img);
  }

  for (auto sampler : samplers_) {
    rt->release_sampler(sampler);
  }

  rt->release_kernel(kernel_);

  after_kernel_exec_.clear();
  arguments_.clear();
  buffers_.clear();
  images_.clear();
  samplers_.clear();
}

cassian::Buffer Helper::create_buffer(size_t size) {
  auto *rt = config.runtime();

  auto buf = rt->create_buffer(size);
  buffers_.push_back(buf);

  return buf;
}

cassian::Image Helper::create_image(const ImageDimensions dim,
                                    const ImageType type,
                                    const ImageFormat format,
                                    const ImageChannelOrder order) {
  auto *rt = config.runtime();

  auto img = rt->create_image(dim, type, format, order);
  images_.push_back(img);

  return img;
}

cassian::Sampler Helper::create_sampler(SamplerCoordinates coordinates,
                                        SamplerAddressingMode address_mode,
                                        SamplerFilterMode filter_mode) {
  auto *rt = config.runtime();

  auto sampler = rt->create_sampler(coordinates, address_mode, filter_mode);
  samplers_.push_back(sampler);

  return sampler;
}

} // namespace detail

void kernel(const std::string &name, const std::string &source,
            const std::string &flags,
            const std::optional<std::string> &spirv_flags) {
  kernel({1, 1, 1}, {1, 1, 1}, name, source, flags, spirv_flags);
}

void kernel(size_t global_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags) {
  kernel({global_work_size, 1, 1}, {1, 1, 1}, name, source, flags, spirv_flags);
}

void kernel(std::array<size_t, 1> global_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags) {
  kernel({global_work_size[0], 1, 1}, {1, 1, 1}, name, source, flags,
         spirv_flags);
}

void kernel(std::array<size_t, 2> global_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags) {
  kernel({global_work_size[0], global_work_size[1], 1}, {1, 1, 1}, name, source,
         flags, spirv_flags);
}

void kernel(std::array<size_t, 3> global_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags) {
  kernel(global_work_size, {1, 1, 1}, name, source, flags, spirv_flags);
}

void kernel(std::array<size_t, 1> global_work_size,
            std::array<size_t, 1> local_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags) {
  kernel({global_work_size[0], 1, 1}, {local_work_size[0], 1, 1}, name, source,
         flags, spirv_flags);
}

void kernel(std::array<size_t, 2> global_work_size,
            std::array<size_t, 2> local_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags) {
  kernel({global_work_size[0], global_work_size[1], 1},
         {local_work_size[0], local_work_size[1], 1}, name, source, flags,
         spirv_flags);
}

void kernel(std::array<size_t, 3> global_work_size,
            std::array<size_t, 3> local_work_size, const std::string &name,
            const std::string &source, const std::string &flags,
            const std::optional<std::string> &spirv_flags) {
  auto &h = detail::Helper::instance();
  auto f = finally([] { detail::Helper::instance().cleanup(); });

  h.kernel(name, source, flags, spirv_flags);
  h.execute(global_work_size, local_work_size);
}

void sampler(SamplerCoordinates coordinates, SamplerAddressingMode address_mode,
             SamplerFilterMode filter_mode) {
  auto &h = detail::Helper::instance();
  h.pass(h.create_sampler(coordinates, address_mode, filter_mode));
}

bool should_skip_test(const Requirements &requirements) {
  auto &h = detail::Helper::instance();
  return cassian::should_skip_test(requirements, *h.config.runtime());
}
} // namespace cassian::test
