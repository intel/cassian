/*
 * Copyright (C) 2021-2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_RUNTIME_HPP
#define CASSIAN_RUNTIME_RUNTIME_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include <cassian/fp_types/bfloat16.hpp>
#include <cassian/fp_types/half.hpp>
#include <cassian/fp_types/tfloat.hpp>
#include <cassian/runtime/access_qualifier.hpp>
#include <cassian/runtime/device_properties.hpp>
#include <cassian/runtime/feature.hpp>
#include <cassian/runtime/image_properties.hpp>
#include <cassian/runtime/program_descriptor.hpp>
#include <cassian/runtime/sampler_properties.hpp>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * API agnostic representation of a compute buffer.
 */
struct Buffer {
  /**
   * Default constructor.
   */
  Buffer() = default;

  /**
   * Construct a buffer with a given id and size.
   *
   * @param[in] id buffer id.
   * @param[in] size buffer size.
   */
  Buffer(std::uintptr_t id, size_t size);

  /**
   * Copy constructor.
   */
  Buffer(const Buffer &) = default;

  /**
   * Move constructor.
   */
  Buffer(Buffer &&) = default;

  /**
   * Destructor.
   */
  ~Buffer() = default;

  /**
   * Copy assignment operator.
   */
  Buffer &operator=(const Buffer &) = default;

  /**
   * Move assignment operator.
   */
  Buffer &operator=(Buffer &&) = default;

  /**
   * API-specifc id for tracking purposes.
   */
  std::uintptr_t id = 0;

  /**
   * Size of a buffer in bytes.
   */
  size_t size = 0;
};

/**
 * API agnostic representation of a compute image.
 */
struct Image {
  /**
   * Default constructor.
   */
  Image() = default;

  /**
   * Construct an Image with a given id and dimensions.
   *
   * @param[in] id image id.
   * @param[in] dim image dimensions(width, height, depth).
   */
  Image(std::uintptr_t id, ImageDimensions dim);

  /**
   * Copy constructor.
   */
  Image(const Image &) = default;

  /**
   * Move constructor.
   */
  Image(Image &&) = default;

  /**
   * Destructor.
   */
  ~Image() = default;

  /**
   * Copy assignment operator.
   */
  Image &operator=(const Image &) = default;

  /**
   * Move assignment operator.
   */
  Image &operator=(Image &&) = default;

  /**
   * API-specifc id for tracking purposes.
   */
  std::uintptr_t id = 0;

  /**
   * Image dimensions
   */
  ImageDimensions dim;
};

/**
 * API agnostic representation of a compute kernel.
 */
struct Kernel {
  /**
   * Default constructor.
   */
  Kernel() = default;

  /**
   * Construct a kernel with a given id.
   *
   * @param[in] id kernel id.
   */
  explicit Kernel(std::uintptr_t id);

  /**
   * Copy constructor.
   */
  Kernel(const Kernel &) = default;

  /**
   * Move constructor.
   */
  Kernel(Kernel &&) = default;

  /**
   * Destructor.
   */
  ~Kernel() = default;

  /**
   * Copy assignment operator.
   */
  Kernel &operator=(const Kernel &) = default;

  /**
   * Move assignment operator.
   */
  Kernel &operator=(Kernel &&) = default;

  /**
   * API-specifc id for tracking purposes.
   */
  std::uintptr_t id = 0;
};

/**
 * API agnostic representation of a compute sampler.
 */
struct Sampler {
  /**
   * Default constructor.
   */
  Sampler() = default;

  /**
   * Construct a sampler with a given id and size.
   *
   * @param[in] id sampler id.
   */
  Sampler(std::uintptr_t id);

  /**
   * Copy constructor.
   */
  Sampler(const Sampler &) = default;

  /**
   * Move constructor.
   */
  Sampler(Sampler &&) = default;

  /**
   * Destructor.
   */
  ~Sampler() = default;

  /**
   * Copy assignment operator.
   */
  Sampler &operator=(const Sampler &) = default;

  /**
   * Move assignment operator.
   */
  Sampler &operator=(Sampler &&) = default;

  /**
   * API-specifc id for tracking purposes.
   */
  std::uintptr_t id = 0;
};

/**
 * API agnostic representation of a local memory.
 */
struct LocalMemory {
  /**
   * Default constructor.
   */
  LocalMemory() = default;

  /**
   * Construct a LocalMemory with a given size.
   *
   * @param[in] size local memory size.
   */
  explicit LocalMemory(size_t size) : size(size) {}

  /**
   * Copy constructor.
   */
  LocalMemory(const LocalMemory &) = default;

  /**
   * Move constructor.
   */
  LocalMemory(LocalMemory &&) = default;

  /**
   * Destructor.
   */
  ~LocalMemory() = default;

  /**
   * Copy assignment operator.
   */
  LocalMemory &operator=(const LocalMemory &) = default;

  /**
   * Move assignment operator.
   */
  LocalMemory &operator=(LocalMemory &&) = default;

  /**
   * Local memory buffer size.
   */
  size_t size = 0;
};

/**
 * Abstract class representing API agnostic runtime.
 */
class Runtime {
public:
  /**
   * Default constructor.
   */
  Runtime() = default;

  /**
   * Copy constructor.
   */
  Runtime(const Runtime &) = default;

  /**
   * Move constructor.
   */
  Runtime(Runtime &&) = default;

  /**
   * Virtual destructor.
   */
  virtual ~Runtime() = default;

  /**
   * Copy assignment operator.
   */
  Runtime &operator=(const Runtime &) = default;

  /**
   * Move assignment operator.
   */
  Runtime &operator=(Runtime &&) = default;

  /**
   * Initialize runtime.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual void initialize() = 0;

  /**
   * Create buffer.
   *
   * @param[in] size size in bytes.
   * @param[in] access access qualifier
   * @returns Buffer object.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual Buffer
  create_buffer(size_t size,
                AccessQualifier access = AccessQualifier::read_write) = 0;

  /**
   * Create image.
   *
   * @param[in] dim ImageDimensions(width, height, depth)
   * @param[in] type ImageType enum value.
   * @param[in] format ImageFormat enum value
   * @param[in] order ImageChannelOrder enum value
   * @param[in] access access qualifier
   * @returns Image object.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual Image
  create_image(const ImageDimensions dim, const ImageType type,
               const ImageFormat format, const ImageChannelOrder order,
               AccessQualifier access = AccessQualifier::read_write) = 0;

  /**
   * Create image plane view.
   *
   * @param[in] image NV12 image object to get plane view
   * @param[in] access access qualifier
   * @returns Image object.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual Image
  get_image_plane(Image image, ImagePlane plane,
                  AccessQualifier access = AccessQualifier::read_only) = 0;

  /**
   * Create sampler.
   *
   * @param[in] coordinates SamplerCoordinates enum value
   * @param[in] address_mode SamplerAddressingMode enum value
   * @param[in] filter_mode SamplerFilterMode enum value
   * @returns Sampler object.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual Sampler create_sampler(
      SamplerCoordinates coordinates = SamplerCoordinates::unnormalized,
      SamplerAddressingMode address_mode = SamplerAddressingMode::clamp_to_edge,
      SamplerFilterMode filter_mode = SamplerFilterMode::nearest) = 0;

  /**
   * Read data from buffer.
   *
   * Blocking buffer read.
   *
   * @param[in] buffer buffer to read.
   * @param[out] data read data.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   * @note data must point to a memory region greater or equal to buffer size.
   */
  virtual void read_buffer(const Buffer &buffer, void *data) = 0;

  /**
   * Read data from image.
   *
   * Blocking image read.
   *
   * @param[in] image image to read.
   * @param[out] data read data.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   * @note data must point to a memory region greater or equal to image size.
   */
  virtual void read_image(const Image &image, void *data) = 0;

  /**
   * Write data to buffer.
   *
   * Blocking buffer write.
   *
   * @param[in] buffer buffer to write.
   * @param[in] data data to write.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   * @note Data must point to a memory region not greater than buffer size.
   */
  virtual void write_buffer(const Buffer &buffer, const void *data) = 0;

  /**
   * Write data to image.
   *
   * Blocking image write.
   *
   * @param[in] image image to write.
   * @param[in] data data to write.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   * @note Data must point to a memory region not greater than image size.
   */
  virtual void write_image(const Image &image, const void *data) = 0;

  /**
   * Release buffer.
   *
   * @param[in] buffer buffer to release.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   * @note After releasing, buffer must not be used anymore.
   * @note Buffer must not be in use when calling this function.
   */
  virtual void release_buffer(const Buffer &buffer) = 0;

  /**
   * Release image.
   *
   * @param[in] image image to release.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   * @note After releasing, image must not be used anymore.
   * @note Image must not be in use when calling this function.
   */
  virtual void release_image(const Image &image) = 0;

  /**
   * Release sampler.
   *
   * @param[in] sampler sampler to release.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   * @note After releasing, sampler must not be used anymore.
   * @note Sampler must not be in use when calling this function.
   */
  virtual void release_sampler(const Sampler &sampler) = 0;

  /**
   * Read buffer to std::vector.
   *
   * @tparam T std::vector type.
   * @param[in] buffer buffer to read.
   * @returns std::vector with read buffer data.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  template <typename T>
  std::vector<T> read_buffer_to_vector(const Buffer &buffer);

  /**
   * Write buffer from std::vector.
   *
   * @tparam T std::vector type.
   * @param[in] buffer buffer to write.
   * @param[in] data vector with data to write.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  template <typename T>
  void write_buffer_from_vector(const Buffer &buffer,
                                const std::vector<T> &data);

  /**
   * Create kernel.
   *
   * @param[in] kernel_name kernel name.
   * @param[in] source kernel source code.
   * @param[in] build_options build options to use during kernel construction.
   * @param[in] program_type program type to use during kernel construction.
   * @returns Kernel object.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual Kernel
  create_kernel(const std::string &kernel_name, const std::string &source,
                const std::string &build_options,
                const std::string &program_type,
                const std::optional<std::string> &spirv_options = std::nullopt,
                bool quiet = false) = 0;

  /**
   * Create kernel from multiple programs.
   *
   * @param[in] kernel_name kernel name.
   * @param[in] program_descriptors program descriptors required for kernel
   * construction.
   * @param[in] linker_options linker options to use for programs linking.
   * @returns Kernel object.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual Kernel create_kernel_from_multiple_programs(
      const std::string &kernel_name,
      const std::vector<ProgramDescriptor> &program_descriptors,
      const std::string &linker_options = "", bool quiet = false) = 0;

  /**
   * Create program and get info.
   *
   * @param[in] source kernel source code.
   * @param[in] build_options build options to use during kernel construction.
   * @param[in] program_type program type to use during kernel construction.
   * @returns Program data bytes.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual std::vector<uint8_t> create_program_and_get_native_binary(
      const std::string &source, const std::string &build_options,
      const std::string &program_type,
      const std::optional<std::string> &spirv_options, bool quiet) = 0;

  /**
   * Set buffer as kernel argument.
   *
   * @param[in] kernel kernel to use.
   * @param[in] argument_index argument index.
   * @param[in] buffer buffer to set as kernel argument.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual void set_kernel_argument(const Kernel &kernel, int argument_index,
                                   const Buffer &buffer) = 0;

  /**
   * Set image as kernel argument.
   *
   * @param[in] kernel kernel to use.
   * @param[in] argument_index argument index.
   * @param[in] image image to set as kernel argument.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual void set_kernel_argument(const Kernel &kernel, int argument_index,
                                   const Image &image) = 0;

  /**
   * Set local memory as kernel argument.
   *
   * @param[in] kernel kernel to use.
   * @param[in] argument_index argument index.
   * @param[in] local_memory size of local memory to set.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  void set_kernel_argument(const Kernel &kernel, int argument_index,
                           const LocalMemory &local_memory) {
    set_kernel_argument(kernel, argument_index, local_memory.size, nullptr);
  }

  /**
   * Set sampler as kernel argument.
   *
   * @param[in] kernel kernel to use.
   * @param[in] argument_index argument index.
   * @param[in] sampler sampler to set as kernel argument.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual void set_kernel_argument(const Kernel &kernel, int argument_index,
                                   const Sampler &sampler) = 0;

  /**
   * Set scalar as a kernel argument.
   *
   * @param[in] kernel kernel to use.
   * @param[in] argument_index argument index.
   * @param[in] argument object to set as the kernel argument.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  template <typename T>
  void set_kernel_argument(const Kernel &kernel, int argument_index,
                           const T &argument) {
    set_kernel_argument(kernel, argument_index, sizeof(argument), &argument);
  }

  /**
   * Run kernel with 3D global work size.
   *
   * @param[in] kernel kernel to run.
   * @param[in] global_work_size global work size.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  void run_kernel(const Kernel &kernel,
                  const std::array<size_t, 3> global_work_size) {
    run_kernel_common(kernel, global_work_size);
  }

  /**
   * Run kernel with 2D global work size.
   *
   * @param[in] kernel kernel to run.
   * @param[in] global_work_size global work size.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  void run_kernel(const Kernel &kernel,
                  const std::array<size_t, 2> global_work_size) {
    run_kernel_common(kernel, {global_work_size[0], global_work_size[1], 1});
  }

  /**
   * Run kernel with 1D global work size.
   *
   * @overload
   */
  void run_kernel(const Kernel &kernel,
                  const std::array<size_t, 1> global_work_size) {
    run_kernel_common(kernel, {global_work_size[0], 1, 1});
  }

  /**
   * Run kernel with 1D global work size.
   *
   * @overload
   */
  void run_kernel(const Kernel &kernel, const size_t global_work_size) {
    run_kernel_common(kernel, {global_work_size, 1, 1});
  }

  /**
   * Run kernel with 3D global and local work size.
   *
   * @param[in] kernel kernel to run.
   * @param[in] global_work_size global work size.
   * @param[in] local_work_size local work size.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  void run_kernel(const Kernel &kernel, std::array<size_t, 3> global_work_size,
                  std::array<size_t, 3> local_work_size) {
    run_kernel_common(kernel, global_work_size, &local_work_size);
  };

  /**
   * Run kernel with 2D global and local work size.
   *
   * @param[in] kernel kernel to run.
   * @param[in] global_work_size global work size.
   * @param[in] local_work_size local work size.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  void run_kernel(const Kernel &kernel, std::array<size_t, 2> global_work_size,
                  const std::array<size_t, 2> local_work_size) {
    std::array<size_t, 3> local_ws = {local_work_size[0], local_work_size[1],
                                      1};
    run_kernel_common(kernel, {global_work_size[0], global_work_size[1], 1},
                      &local_ws);
  };

  /**
   * Run kernel with 1D global and local work size.
   *
   * @overload
   */
  void run_kernel(const Kernel &kernel,
                  const std::array<size_t, 1> global_work_size,
                  const std::array<size_t, 1> local_work_size) {
    std::array<size_t, 3> local_ws = {local_work_size[0], 1, 1};
    run_kernel_common(kernel, {global_work_size[0], 1, 1}, &local_ws);
  }

  /**
   * Run kernel with 1D global and local work size.
   *
   * @overload
   */
  void run_kernel(const Kernel &kernel, const size_t global_work_size,
                  const size_t local_work_size) {
    std::array<size_t, 3> local_ws = {local_work_size, 1, 1};
    run_kernel_common(kernel, {global_work_size, 1, 1}, &local_ws);
  }

  /**
   * Release kernel.
   *
   * @param[in] kernel kernel to release.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   * @note After releasing, kernel must not be used anymore.
   * @note Kernel must not be in use when calling this function.
   */
  virtual void release_kernel(const Kernel &kernel) = 0;

  /**
   * Check if a given feature is supported.
   *
   * @param[in] feature feature to check.
   * @returns true if feature is supported.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual bool is_feature_supported(Feature feature) const = 0;

  /**
   * Get device property.
   *
   * @param[in] property property to read.
   * @returns value of a given property.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual int get_device_property(DeviceProperty property) const = 0;

  /**
   * Runtime name.
   *
   * @returns runtime name.
   */
  virtual std::string name() const = 0;

  /**
   * Query the device for maximum work size and return suggested local work
   * size according to provided global work size. Distributes available work
   * size from the first dimension to the last.
   *
   * @param[in] global_work_size global work size to use, work size in each
   * dimension has to be greater than or equal to 1.
   * @param[in] max_sizes max device work size available in each dimension
   * @param[in] max_work_size work size available
   * @returns local work size.
   */
  template <size_t N>
  static std::array<size_t, N>
  get_max_local_work_size(const std::array<size_t, N> &global_work_size,
                          const std::array<size_t, N> &max_sizes,
                          const size_t &max_work_size) {
    auto global_ws = global_work_size;

    if (global_ws < max_sizes) {
      size_t multiplier = 1;
      std::for_each(global_ws.begin(), global_ws.end(),
                    [&multiplier](size_t e) { multiplier *= e; });
      if (multiplier <= max_work_size) {
        return global_ws;
      }
    }

    size_t remaining_size = max_work_size;
    size_t size_for_this_one = 0;
    for (auto i = 0U; i < N; i++) {
      auto &size = global_ws.at(i);
      size_for_this_one = size > remaining_size ? remaining_size : size;
      while ((size % size_for_this_one) != 0 ||
             (size_for_this_one > max_sizes.at(i))) {
        size_for_this_one--;
      }
      size = size_for_this_one;
      remaining_size /= size;
    }
    return global_ws;
  }

  /**
   * Query the device for maximum work size and return suggested local work
   * size according to provided global work size.
   *
   * @param[in] global_work_size global work size to use
   * @param[in] max_sizes max device work size available
   * @param[in] max_work_size work size available
   * @returns local work size.
   * @overload
   */
  static size_t get_max_local_work_size(const size_t &global_work_size,
                                        const size_t &max_sizes,
                                        const size_t &max_work_size) {
    const std::array<size_t, 1> tmp_global_work_size = {global_work_size};
    const std::array<size_t, 1> tmp_max_sizes = {max_sizes};
    return get_max_local_work_size(tmp_global_work_size, tmp_max_sizes,
                                   max_work_size)[0];
  }

protected:
  /**
   * Set value as a kernel argument.
   *
   * @param[in] kernel kernel to use.
   * @param[in] argument_index argument index.
   * @param[in] argument pointer to the argument data to set as kernel argument.
   * @param[in] argument_size size of the kernel argument.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual void set_kernel_argument(const Kernel &kernel, int argument_index,
                                   size_t argument_size,
                                   const void *argument) = 0;

  /**
   * Run kernel with 3D global and local work size.
   *
   * @param[in] kernel kernel to run.
   * @param[in] global_work_size global work size.
   * @param[in] local_work_size local work size.
   * @throws cassian::RuntimeException Thrown if runtime encountered a fatal
   * error.
   */
  virtual void
  run_kernel_common(const Kernel &kernel,
                    std::array<size_t, 3> global_work_size,
                    const std::array<size_t, 3> *local_work_size = nullptr) = 0;
};

template <typename T>
std::vector<T> Runtime::read_buffer_to_vector(const Buffer &buffer) {
  const size_t elements = buffer.size / sizeof(T);
  std::vector<T> output(elements);
  read_buffer(buffer, output.data());
  return output;
}

/**
 * Specialization for Bfloat16.
 */
template <>
std::vector<Bfloat16>
Runtime::read_buffer_to_vector<Bfloat16>(const Buffer &buffer);

/**
 * Specialization for Half.
 */
template <>
std::vector<Half> Runtime::read_buffer_to_vector<Half>(const Buffer &buffer);

/**
 * Specialization for Tfloat.
 */
template <>
std::vector<Tfloat>
Runtime::read_buffer_to_vector<Tfloat>(const Buffer &buffer);

template <typename T>
void Runtime::write_buffer_from_vector(const Buffer &buffer,
                                       const std::vector<T> &data) {
  write_buffer(buffer, data.data());
}

/**
 * Specialization for Bfloat16.
 */
template <>
void Runtime::write_buffer_from_vector<Bfloat16>(
    const Buffer &buffer, const std::vector<Bfloat16> &data);

/**
 * Specialization for Half.
 */
template <>
void Runtime::write_buffer_from_vector<Half>(const Buffer &buffer,
                                             const std::vector<Half> &data);

/**
 * Specialization for Tfloat.
 */
template <>
void Runtime::write_buffer_from_vector<Tfloat>(const Buffer &buffer,
                                               const std::vector<Tfloat> &data);

/**
 * Exception class used when a runtime encounters a fatal error.
 */
class RuntimeException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

/**
 * Get C for Metal type name.
 *
 * @tparam T the host type.
 * @returns C for Metal type name.
 */
template <typename T> std::string to_cm_string();

} // namespace cassian
#endif
