/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_UTILITY_UTILITY_HPP
#define CASSIAN_UTILITY_UTILITY_HPP

#include <cassert>
#include <cassian/vector/vector.hpp>
#include <cstddef>
#include <cstdint>
#include <cstring>
#ifdef CASSIAN_OLD_GNU
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Load text file.
 *
 * @param[in] file_path path to a file to read.
 * @returns text read from a file.
 */
std::string load_text_file(const std::string &file_path);

/**
 * Save text file.
 *
 * @param[in] data text to save.
 * @param[in] file_path path to output file.
 */
void save_text_file(const std::string &data, const std::string &file_path);

/**
 * Load binary file.
 *
 * @param[in] file_path path to a file to read.
 * @returns bytes read from a file.
 */
std::vector<uint8_t> load_binary_file(const std::string &file_path);

/**
 * Save binary file.
 *
 * @param[in] data bytes to save.
 * @param[in] file_path path to output file.
 */
void save_binary_file(const std::vector<uint8_t> &data,
                      const std::string &file_path);

/**
 * Combine bytes from multiple elements into single, larger element.
 *
 * @tparam IN input data type.
 * @tparam OUT output data type.
 * @param[in] input elements to combine.
 * @returns combined elements.
 */
template <typename OUT, typename IN>
OUT combine_bytes(const std::vector<IN> &input) {
  const size_t bits = 8;
  OUT output = 0;
  for (size_t i = 0; i < input.size(); ++i) {
    output |= uint8_t(input[i]) << (i * bits);
  }
  return output;
}

/**
 * Split bytes from single, large element into multiple smaller elements.
 *
 * @tparam IN input data type.
 * @tparam OUT output data type.
 * @param[in] input element to split.
 * @returns splitted elements.
 */
template <typename OUT, typename IN>
std::vector<OUT> split_bytes(const IN input) {
  const size_t iterations = sizeof(IN) / sizeof(OUT);
  const uint8_t mask = 0xFF;
  const size_t bits = 8;
  std::vector<OUT> output(iterations);
  for (size_t i = 0; i < iterations; ++i) {
    output[i] = (input >> bits * i) & mask;
  }
  return output;
}

/**
 * Get application path.
 *
 * @returns full path to an application.
 * @throws cassian::PathNotFoundException Thrown if application path logic
 * returns an error
 */
fs::path get_application_directory();

/**
 * Exception class used when a given application path is not found in the
 * installation directory.
 */
class PathNotFoundException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

/**
 * Get full path to an asset.
 *
 * Search order:
 *  - current working directory;
 *  - ../share/cassian/;
 *  - application directory
 *  - application directory + ../share/cassian/
 *  - application directory + ../
 *
 * @param[in] asset_path path to an asset inside the installation directory.
 * @returns full path to an asset.
 * @throws cassian::AssetNotFoundException Thrown if `asset_path` does not point
 * to a valid asset path.
 */
std::string get_asset(const std::string &asset_path);

/**
 * Exception class used when a given asset is not found in the installation
 * directory.
 */
class AssetNotFoundException : public std::runtime_error {
  using std::runtime_error::runtime_error;
};

/**
 * Pack a vector into larger data type.
 *
 * @tparam INPUT_TYPE the type of input vector.
 * @tparam OUTPUT_TYPE the type of output vector.
 * @param[in] input input vector.
 * @param[in] step step used to iterate over input vector, after fully packing
 *                 single OUTPUT_TYPE iterator returns to first unpacked
 *                 INPUT_TYPE.
 * @returns packed vector.
 */
template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE> pack_vector(const std::vector<INPUT_TYPE> &input,
                                     int step = 1) {
  static_assert(sizeof(OUTPUT_TYPE) >= sizeof(INPUT_TYPE),
                "Size of OUTPUT_TYPE must be greater or equal to INPUT_TYPE");
  assert(step > 0);

  const size_t size_ratio = sizeof(OUTPUT_TYPE) / sizeof(INPUT_TYPE);

  std::vector<OUTPUT_TYPE> output(input.size() / size_ratio);

  const auto columns = static_cast<size_t>(step);
  const size_t rows = output.size() / columns;

  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < columns; ++x) {
      std::vector<INPUT_TYPE> tmp(size_ratio);
      for (size_t i = 0; i < tmp.size(); ++i) {
        const size_t row_offset = y * columns * size_ratio;
        const size_t column_offset = x;
        const size_t element_offset = i * columns;
        const size_t input_offset = row_offset + column_offset + element_offset;
        tmp[i] = input[input_offset];
      }
      const size_t output_offset = y * columns + x;
      std::memcpy(&output[output_offset], tmp.data(), sizeof(OUTPUT_TYPE));
    }
  }

  return output;
}

/**
 * Pack a vector into larger data type. Overload when packing into
 * cassian::Vector
 *
 * @overload
 */
template <typename VECTOR>
std::vector<Vector<typename VECTOR::value_type, VECTOR::vector_size,
                   VECTOR::size_in_memory>>
pack_vector(const std::vector<typename VECTOR::value_type> &input,
            int step = 1) {
  assert(step > 0);

  std::vector<Vector<typename VECTOR::value_type, VECTOR::vector_size,
                     VECTOR::size_in_memory>>
      output(input.size() / VECTOR::vector_size);

  const auto columns = static_cast<size_t>(step);
  const size_t rows = output.size() / columns;

  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < columns; ++x) {
      std::vector<typename VECTOR::value_type> tmp(VECTOR::vector_size);
      for (size_t i = 0; i < VECTOR::vector_size; ++i) {
        const size_t row_offset = y * columns * VECTOR::vector_size;
        const size_t column_offset = x;
        const size_t element_offset = i * columns;
        const size_t input_offset = row_offset + column_offset + element_offset;
        tmp[i] = input[input_offset];
      }
      const size_t output_offset = y * columns + x;
      std::memcpy(&output[output_offset], tmp.data(),
                  sizeof(typename VECTOR::value_type) * VECTOR::vector_size);
    }
  }
  return output;
}

/**
 * Unpack a vector into smaller data type. Reverses pack_vector function.
 *
 * @tparam INPUT_TYPE the type of input vector.
 * @tparam OUTPUT_TYPE the type of output vector.
 * @param[in] input input vector.
 * @param[in] step step used to iterate over input vector.
 * @returns unpacked vector.
 */
template <typename OUTPUT_TYPE, typename INPUT_TYPE>
std::vector<OUTPUT_TYPE> unpack_vector(const std::vector<INPUT_TYPE> &input,
                                       int step = 1) {
  static_assert(sizeof(INPUT_TYPE) >= sizeof(OUTPUT_TYPE),
                "Size of INPUT_TYPE must be greater or equal to OUTPUT_TYPE");
  assert(step > 0);

  const size_t size_ratio = sizeof(INPUT_TYPE) / sizeof(OUTPUT_TYPE);

  std::vector<OUTPUT_TYPE> output(input.size() * size_ratio);

  const auto columns = static_cast<size_t>(step);
  const size_t rows = input.size() / columns;

  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < columns; ++x) {
      std::vector<OUTPUT_TYPE> tmp(size_ratio);
      const size_t input_offset = y * columns + x;
      std::memcpy(tmp.data(), &input[input_offset], sizeof(INPUT_TYPE));
      size_t output_offset = x + y * columns * size_ratio;
      for (size_t i = 0; i < tmp.size(); ++i) {
        output[output_offset] = tmp[i];
        output_offset += columns;
      }
    }
  }
  return output;
}

/**
 * Unpack a vector into smaller data type. Overload when unpacking from
 * cassian::Vector
 *
 * @overload
 */
template <typename OUTPUT_TYPE, typename VALUE_TYPE, int N, int SIZE_IN_MEMORY>
std::vector<OUTPUT_TYPE>
unpack_vector(const std::vector<Vector<VALUE_TYPE, N, SIZE_IN_MEMORY>> &input,
              const int step = 1) {
  static_assert(std::is_same<OUTPUT_TYPE, VALUE_TYPE>::value,
                "When unpacking ca::Vector OUTPUT_TYPE needs to be same as "
                "VECTOR::value_type");
  assert(step > 0);

  std::vector<VALUE_TYPE> output(input.size() * N);

  const auto columns = static_cast<size_t>(step);
  const size_t rows = input.size() / columns;

  for (size_t y = 0; y < rows; ++y) {
    for (size_t x = 0; x < columns; ++x) {
      std::vector<OUTPUT_TYPE> tmp(N);
      const size_t input_offset = y * columns + x;
      std::memcpy(tmp.data(), &input[input_offset], sizeof(VALUE_TYPE) * N);
      size_t output_offset = x + y * columns * N;
      for (size_t i = 0; i < tmp.size(); ++i) {
        output[output_offset] = tmp[i];
        output_offset += columns;
      }
    }
  }
  return output;
}

/**
 * Convert array<T, N> to string.
 *
 * @tparam T the type of std::array message to log.
 * @tparam N the size of std::array
 * @param[in] arr array to convert.
 * @returns array converted string.
 */
template <typename T, size_t N>
std::string to_string(const std::array<T, N> &arr) {
  std::stringstream ss;
  ss << '{';
  for (size_t i = 0; i < N; i++) {
    ss << std::to_string(arr.at(i)) << (i + 1 >= N ? "" : ", ");
  }
  ss << '}';
  return ss.str();
}

/**
 * Convert back slashes to forward slashes in a string.
 *
 * @param[in] str string to convert.
 * @returns converted string.
 */
std::string convert_to_forward_slashes(const std::string &str);

/**
 * Convert typed-value to string. Switches between arithmetic std and
 * cassian to_string function
 *
 * @tparam T the type to log.
 * @param[in] value typed value to convert.
 * @returns string returned from another to_string function.
 */
template <typename T> std::string to_string(const T &value) {
  if constexpr (std::is_arithmetic_v<T>) {
    return std::to_string(value);
  } else {
    return to_string(value);
  }
}

/**
 * FinalActions class calling user-defined action in dtor
 *
 * @tparam A the type for callable object
 */
template <typename A> class FinalAction {
public:
  /**
   * Constructor from a callable object
   *
   * @param[in] a callable object
   */
  explicit FinalAction(A a) : act{a} {}

  FinalAction(const FinalAction &) = delete;
  FinalAction(FinalAction &&) = delete;
  FinalAction &operator=(const FinalAction &) = delete;
  FinalAction &operator=(FinalAction &&) = delete;

  ~FinalAction() { act(); }

private:
  A act;
};

/**
 * Creates FinalAction object instance
 *
 * @tparam A type for callable object
 * @param[in] act callable object
 */
template <typename A> FinalAction<A> finally(A act) {
  return FinalAction<A>{act};
}

} // namespace cassian
#endif
