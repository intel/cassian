/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_VECTOR_VECTOR_HPP
#define CASSIAN_VECTOR_VECTOR_HPP

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Exception class used when a Vector can't be constructed with a given number
 * of elements.
 */
class VectorBadNumberOfElementsException : public std::logic_error {
public:
  /**
   * Construct exception with preformatted error message.
   *
   * @param[in] source source of the invalid number of elements.
   * @param[in] expected expected number of elements.
   * @param[in] actual actual number of elements.
   */
  VectorBadNumberOfElementsException(const std::string &source, int expected,
                                     int actual)
      : std::logic_error(error_message(source, expected, actual)) {}

private:
  std::string static error_message(const std::string &source, int expected,
                                   int actual);
};

/**
 * Exception class used when a trying to access element at index greater or
 * equal vector size
 */
class VectorAccessViolationException : public std::logic_error {
public:
  /**
   * Construct exception with preformatted error message.
   *
   * @param[in] index accessed index.
   * @param[in] size size of vector.
   */
  VectorAccessViolationException(int index, int size)
      : std::logic_error(error_message(index, size)) {}

private:
  std::string static error_message(int index, int size);
};

/**
 * Static vector implementation.
 *
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename T, int N, int SIZE_IN_MEMORY = N> class Vector {
public:
  /**
   * Default constructor.
   */
  Vector() = default;

  /**
   * Construct Vector from std::initializer_list.
   *
   * @param[in] list std::initializer_list from which Vector should be
   * constructed.
   * @throws cassian::VectorBadNumberOfElementsException Thrown if list has
   * different number of elements than constructed Vector.
   */
  Vector(std::initializer_list<T> list) {
    const int list_size = static_cast<int>(list.size());
    if (list_size == N) {
      std::copy(list.begin(), list.end(), data_.begin());
    } else {
      throw VectorBadNumberOfElementsException("initializer list", N,
                                               list_size);
    }
  }

  /**
   * Construct Vector from std::vector.
   *
   * @param[in] vector std::vector from which Vector should be constructed.
   * @throws cassian::VectorBadNumberOfElementsException Thrown if vector has
   * different number of elements than constructed Vector.
   */
  explicit Vector(std::vector<T> vector) {
    const int vector_size = static_cast<int>(vector.size());
    if (vector.size() == N) {
      std::move(vector.begin(), vector.end(), data_.begin());
    } else {
      throw VectorBadNumberOfElementsException("std::vector", N, vector_size);
    }
  }

  /**
   * Construct Vector from a scalar.
   *
   * @param[in] scalar value which will be used to fill a Vector.
   */
  explicit Vector(T scalar) { std::fill(data_.begin(), data_.end(), scalar); }

  /**
   * Copy constructor.
   */
  Vector(const Vector &) = default;

  /**
   * Move constructor.
   */
  Vector(Vector &&) noexcept = default;

  /**
   * Destructor.
   */
  ~Vector() = default;

  /**
   * Copy assignment operator.
   */
  Vector &operator=(const Vector &) = default;

  /**
   * Move assignment operator.
   */
  Vector &operator=(Vector &&) noexcept = default;

  /**
   * Equal operator.
   *
   * @param[in] rhs Vector to compare against.
   * @returns true if all values are equal.
   */
  bool operator==(const Vector &rhs) const {
    return std::equal(data_.begin(), data_.begin() + vector_size,
                      rhs.data_.begin(), rhs.data_.begin() + vector_size);
  };

  /**
   * Equal operator.
   *
   * @param[in] lhs scalar to compare.
   * @param[in] rhs Vector to compare.
   * @returns true if all values are equal.
   */
  friend bool operator==(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    return lhs_vector == rhs;
  }

  /**
   * Equal operator.
   *
   * @param[in] lhs Vector to compare.
   * @param[in] rhs scalar to compare.
   * @returns true if all values are equal.
   */
  friend bool operator==(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    return lhs == rhs_vector;
  }

  /**
   * Not equal operator.
   *
   * @param[in] rhs Vector to compare against.
   * @returns true if any value is not equal.
   */
  bool operator!=(const Vector &rhs) const { return !(*this == rhs); };

  /**
   * Not equal operator.
   *
   * @param[in] lhs scalar to compare.
   * @param[in] rhs Vector to compare.
   * @returns true if any value is not equal.
   */
  friend bool operator!=(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    return lhs_vector != rhs;
  }

  /**
   * Not equal operator.
   *
   * @param[in] lhs Vector to compare.
   * @param[in] rhs scalar to compare.
   * @returns true if any value is not equal.
   */
  friend bool operator!=(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    return lhs != rhs_vector;
  }

  /**
   * Less than operator.
   *
   * @param[in] rhs Vector to compare.
   * @returns true if all lhs values are less than rhs values.
   */
  bool operator<(const Vector &rhs) const {
    return std::equal(data_.begin(), data_.begin() + vector_size,
                      rhs.data_.begin(), rhs.data_.begin() + vector_size,
                      [](T a, T b) -> bool { return a < b; });
  };

  /**
   * Less than operator.
   *
   * @param[in] lhs scalar to compare.
   * @param[in] rhs Vector to compare.
   * @returns true if all lhs values are less than rhs values.
   */
  friend bool operator<(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    return lhs_vector < rhs;
  }

  /**
   * Less than operator.
   *
   * @param[in] lhs Vector to compare.
   * @param[in] rhs scalar to compare.
   * @returns true if all lhs values are less than rhs values.
   */
  friend bool operator<(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    return lhs < rhs_vector;
  }

  /**
   * Greater than operator.
   *
   * @param[in] rhs Vector to compare.
   * @returns true if all lhs values are greater than rhs values.
   */
  bool operator>(const Vector &rhs) const {
    return std::equal(data_.begin(), data_.begin() + vector_size,
                      rhs.data_.begin(), rhs.data_.begin() + vector_size,
                      [](T a, T b) -> bool { return a > b; });
  };

  /**
   * Greater than operator.
   *
   * @param[in] lhs scalar to compare.
   * @param[in] rhs Vector to compare.
   * @returns true if all lhs values are greater than rhs values.
   */
  friend bool operator>(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    return lhs_vector > rhs;
  }

  /**
   * Greater than operator.
   *
   * @param[in] lhs Vector to compare.
   * @param[in] rhs scalar to compare.
   * @returns true if all lhs values are greater than rhs values.
   */
  friend bool operator>(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    return lhs > rhs_vector;
  }

  /**
   * Less than or equal operator.
   *
   * @param[in] rhs Vector to compare.
   * @returns true if all lhs values are less than or equal to rhs values.
   */
  bool operator<=(const Vector &rhs) const {
    return std::equal(data_.begin(), data_.begin() + vector_size,
                      rhs.data_.begin(), rhs.data_.begin() + vector_size,
                      [](T a, T b) -> bool { return a <= b; });
  };

  /**
   * Less than or equal operator.
   *
   * @param[in] lhs scalar to compare.
   * @param[in] rhs Vector to compare.
   * @returns true if all lhs values are less than or equal to rhs values.
   */
  friend bool operator<=(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    return lhs_vector <= rhs;
  }

  /**
   * Less than or equal operator.
   *
   * @param[in] lhs Vector to compare.
   * @param[in] rhs scalar to compare.
   * @returns true if all lhs values are less than or equal to rhs values.
   */
  friend bool operator<=(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    return lhs <= rhs_vector;
  }

  /**
   * Greater than or equal operator.
   *
   * @param[in] rhs Vector to compare.
   * @returns true if all lhs values are greater than or equal to rhs values.
   */
  bool operator>=(const Vector &rhs) const {
    return std::equal(data_.begin(), data_.begin() + vector_size,
                      rhs.data_.begin(), rhs.data_.begin() + vector_size,
                      [](T a, T b) -> bool { return a >= b; });
  };

  /**
   * Greater than or equal operator.
   *
   * @param[in] lhs scalar to compare.
   * @param[in] rhs Vector to compare.
   * @returns true if all lhs values are greater than or equal to rhs values.
   */
  friend bool operator>=(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    return lhs_vector >= rhs;
  }

  /**
   * Greater than or equal operator.
   *
   * @param[in] lhs Vector to compare.
   * @param[in] rhs scalar to compare.
   * @returns true if all lhs values are greater than or equal to rhs values.
   */
  friend bool operator>=(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    return lhs >= rhs_vector;
  }

  /**
   * Addition assignment operator.
   *
   * @param[in] rhs Vector to add.
   * @returns this after addition.
   */
  Vector &operator+=(const Vector &rhs) {
    std::transform(data_.begin(), data_.begin() + vector_size,
                   rhs.data_.begin(), data_.begin(), std::plus<T>());
    return *this;
  }

  /**
   * Addition operator.
   *
   * @param[in] lhs Vector to add.
   * @param[in] rhs Vector to add.
   * @returns result of addition.
   */
  friend Vector operator+(Vector lhs, const Vector &rhs) {
    lhs += rhs;
    return lhs;
  }

  /**
   * Addition operator.
   *
   * @param[in] lhs scalar to add.
   * @param[in] rhs Vector to add.
   * @returns result of addition.
   * @overload
   */
  friend Vector operator+(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    lhs_vector += rhs;
    return lhs_vector;
  }

  /**
   * Addition operator.
   *
   * @param[in] lhs Vector to add.
   * @param[in] rhs scalar to add.
   * @returns result of addition.
   * @overload
   */
  friend Vector operator+(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    lhs += rhs_vector;
    return lhs;
  }

  /**
   * Subtraction assignment operator.
   *
   * @param[in] rhs Vector to subtract.
   * @returns this after subtraction.
   */
  Vector &operator-=(const Vector &rhs) {
    std::transform(data_.begin(), data_.begin() + vector_size,
                   rhs.data_.begin(), data_.begin(), std::minus<T>());
    return *this;
  }

  /**
   * Subtraction operator.
   *
   * @param[in] lhs Vector to subtract.
   * @param[in] rhs Vector to subtract.
   * @returns result of subtraction.
   */
  friend Vector operator-(Vector lhs, const Vector &rhs) {
    lhs -= rhs;
    return lhs;
  }

  /**
   * Unary minus operator.
   *
   * @param[in] lhs Vector to negate.
   * @returns result of negation.
   */
  friend Vector operator-(const Vector &lhs) {
    Vector v(lhs);
    std::transform(v.data_.begin(), v.data_.begin() + v.vector_size,
                   v.data_.begin(), std::negate<T>());
    return v;
  }

  /**
   * Subtraction operator.
   *
   * @param[in] lhs scalar to subtract.
   * @param[in] rhs Vector to subtract.
   * @returns result of subtraction.
   * @overload
   */
  friend Vector operator-(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    lhs_vector -= rhs;
    return lhs_vector;
  }

  /**
   * Subtraction operator.
   *
   * @param[in] lhs Vector to subtract.
   * @param[in] rhs scalar to subtract.
   * @returns result of subtraction.
   * @overload
   */
  friend Vector operator-(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    lhs -= rhs_vector;
    return lhs;
  }

  /**
   * Multiplication assignment operator.
   *
   * @param[in] rhs Vector to multiply.
   * @returns this after multiplication.
   */
  Vector &operator*=(const Vector &rhs) {
    std::transform(data_.begin(), data_.begin() + vector_size,
                   rhs.data_.begin(), data_.begin(), std::multiplies<T>());
    return *this;
  }

  /**
   * Multiplication operator.
   *
   * @param[in] lhs Vector to multiply.
   * @param[in] rhs Vector to multiply.
   * @returns result of multiplication.
   */
  friend Vector operator*(Vector lhs, const Vector &rhs) {
    lhs *= rhs;
    return lhs;
  }

  /**
   * Multiplication operator.
   *
   * @param[in] lhs scalar to multiply.
   * @param[in] rhs Vector to multiply.
   * @returns result of multiplication.
   * @overload
   */
  friend Vector operator*(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    lhs_vector *= rhs;
    return lhs_vector;
  }

  /**
   * Multiplication operator.
   *
   * @param[in] lhs Vector to multiply.
   * @param[in] rhs scalar to multiply.
   * @returns result of multiplication.
   * @overload
   */
  friend Vector operator*(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    lhs *= rhs_vector;
    return lhs;
  }

  /**
   * Division assignment operator.
   *
   * @param[in] rhs Vector to divide.
   * @returns this after division.
   */
  Vector &operator/=(const Vector &rhs) {
    std::transform(data_.begin(), data_.begin() + vector_size,
                   rhs.data_.begin(), data_.begin(), std::divides<T>());
    return *this;
  }

  /**
   * Division operator.
   *
   * @param[in] lhs Vector to divide.
   * @param[in] rhs Vector to divide.
   * @returns result of division.
   */
  friend Vector operator/(Vector lhs, const Vector &rhs) {
    lhs /= rhs;
    return lhs;
  }

  /**
   * Division operator.
   *
   * @param[in] lhs scalar to divide.
   * @param[in] rhs Vector to divide.
   * @returns result of division.
   * @overload
   */
  friend Vector operator/(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    lhs_vector /= rhs;
    return lhs_vector;
  }

  /**
   * Division operator.
   *
   * @param[in] lhs Vector to divide.
   * @param[in] rhs scalar to divide.
   * @returns result of division.
   * @overload
   */
  friend Vector operator/(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    lhs /= rhs_vector;
    return lhs;
  }

  /**
   * Modulo assignment operator.
   *
   * @param[in] rhs Vector to modulo.
   * @returns this after modulo.
   */
  Vector &operator%=(const Vector &rhs) {
    static_assert(std::is_integral<T>::value,
                  "Modulo operator is defined only for integral types");
    std::transform(data_.begin(), data_.begin() + vector_size,
                   rhs.data_.begin(), data_.begin(), std::modulus<T>());
    return *this;
  }

  /**
   * Modulo operator.
   *
   * @param[in] lhs Vector to module.
   * @param[in] rhs Vector to module.
   * @returns result of module.
   */
  friend Vector operator%(Vector lhs, const Vector &rhs) {
    lhs %= rhs;
    return lhs;
  }

  /**
   * Modulo operator.
   *
   * @param[in] lhs scalar to modulo.
   * @param[in] rhs Vector to modulo.
   * @returns result of modulo.
   * @overload
   */
  friend Vector operator%(const T &lhs, const Vector &rhs) {
    Vector lhs_vector(lhs);
    lhs_vector %= rhs;
    return lhs_vector;
  }

  /**
   * Modulo operator.
   *
   * @param[in] lhs Vector to modulo.
   * @param[in] rhs scalar to modulo.
   * @returns result of modulo.
   * @overload
   */
  friend Vector operator%(Vector lhs, const T &rhs) {
    Vector rhs_vector(rhs);
    lhs %= rhs_vector;
    return lhs;
  }

  /**
   * Pre-increment operator.
   *
   * @returns this after increment operation.
   */
  Vector &operator++() {
    std::for_each(data_.begin(), data_.begin() + vector_size,
                  [](T &a) { ++a; });
    return *this;
  }

  /**
   * Post-increment operator.
   *
   * @returns Vector after increment operation.
   */
  Vector operator++(int) {
    Vector output(*this);
    ++(*this);
    return output;
  }

  /**
   * Pre-decrement operator.
   *
   * @returns this after decrement operation.
   */
  Vector &operator--() {
    std::for_each(data_.begin(), data_.begin() + vector_size,
                  [](T &a) { --a; });
    return *this;
  }

  /**
   * Post-decrement operator.
   *
   * @returns Vector after decrement operation.
   */
  Vector operator--(int) {
    Vector output(*this);
    --(*this);
    return output;
  }

  /**
   * Size of vector.
   *
   * @returns number of elements in vector.
   */
  int size() const { return N; }

  /**
   * Get element from vector.
   *
   * @param[in] index index of an element to return.
   * @returns vector element.
   */
  T &operator[](int index) {
    if (index < N) {
      return data_[index];
    }
    throw VectorAccessViolationException(index, N);
  }

  /**
   * Get iterator of a vector beginning.
   *
   */
  constexpr auto begin() const { return data_.begin(); }

  /**
   * Get const element from vector.
   *
   * @overload
   */
  const T &operator[](int index) const {
    if (index < N) {
      return data_[index];
    }
    throw VectorAccessViolationException(index, N);
  }

  /**
   * Vector type.
   */
  using value_type = T;

  /**
   * Compile-time vector size.
   */
  constexpr static int vector_size = N;

  /**
   * Compile-time size in memory.
   */
  constexpr static int size_in_memory = SIZE_IN_MEMORY;

  /**
   * Metadata needed to distinguish vector-like data types.
   */
  using is_vector = void; // used for template metaprograming

private:
  std::array<T, SIZE_IN_MEMORY> data_;
};

/**
 * Convert Vector to string representation.
 *
 * @param[in] value object to convert.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename T, int N, int SIZE_IN_MEMORY = N>
std::string to_string(const Vector<T, N, SIZE_IN_MEMORY> &value) {
  std::stringstream ss;
  ss << "{";
  for (size_t i = 0; i < value.size(); ++i) {
    ss << std::to_string(+value[i]);
    if (i < value.size() - 1) {
      ss << ", ";
    }
  }
  ss << "}";
  return ss.str();
}

/**
 * Append string representation of Vector object to a stream.
 *
 * @param[in] os stream to use.
 * @param[in] value object to convert.
 * @returns used stream.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename T, int N, int SIZE_IN_MEMORY = N>
std::ostream &operator<<(std::ostream &os,
                         const Vector<T, N, SIZE_IN_MEMORY> &value) {
  os << to_string(value);
  return os;
}

/**
 * Meta class for vector types.
 *
 * @tparam T scalar type.
 */
template <typename T, typename ENABLE = void> struct IsVector {
  /**
   * Scalar type.
   */
  static const bool value = false;
};

/**
 * Meta class for vector types.
 *
 * @tparam T vector type.
 */
template <typename T> struct IsVector<T, typename T::is_vector> {
  /**
   * Vector type.
   */
  static const bool value = true;
};

/**
 * Returns true if T is a Vector class.
 *
 * @tparam T type.
 */
template <typename T> constexpr bool is_vector_v = IsVector<T>::value;

/**
 * Metafunction to enable a function declaration if T is a Vector.
 *
 * @tparam T type.
 */
template <typename T>
using EnableIfIsVector = std::enable_if_t<is_vector_v<T>, int>;

/**
 * Metafunction to enable a function declaration if T is a scalar.
 *
 * @tparam T type.
 */
template <typename T>
using EnableIfIsScalar = std::enable_if_t<!is_vector_v<T>, int>;

/**
 * Metafunction to enable a function declaration if T is integral.
 *
 * @tparam T type.
 */
template <typename T>
using EnableIfIsIntegral = std::enable_if_t<std::is_integral<T>::value, int>;

/**
 * Metafunction to enable a function declaration if T is floating point.
 *
 * @tparam T type.
 */
template <typename T>
using EnableIfIsFloatingPoint =
    std::enable_if_t<std::is_floating_point<T>::value, int>;

/**
 * Meta class for scalar types.
 *
 * @tparam T scalar type.
 */
template <typename T, typename ENABLE = void> struct ScalarType {
  /**
   * Scalar type.
   */
  using value = T;
};

/**
 * Meta class for scalar types.
 *
 * @tparam T vector type.
 */
template <typename T> struct ScalarType<T, typename T::is_vector> {
  /**
   * Scalar type.
   */
  using value = typename T::value_type;
};

/**
 * Scalar type.
 */
template <typename T> using scalar_type_v = typename ScalarType<T>::value;

/**
 * MakeSigned works same as std::make_signed but works with cassian::Vector
 */
template <typename T, typename ENABLE = void> struct MakeSigned {
  /**
   * Make signed
   */
  using type = std::make_signed_t<T>;
};
/**
 * MakeSigned works as std::make_signed but works with cassian::Vector
 */
template <typename T> struct MakeSigned<T, typename T::is_vector> {
  /**
   * Make signed
   */
  using type = Vector<std::make_signed_t<typename T::value_type>,
                      T::vector_size, T::size_in_memory>;
};
/**
 * cassian::make_signed_t works same as std::make_signed_t but it works with
 * cassian::Vector
 */
template <typename T> using make_signed_t = typename MakeSigned<T>::type;

/**
 * MakeUnsigned works as std::make_unsigned but works with cassian::Vector
 */
template <typename T, typename ENABLE = void> struct MakeUnsigned {
  /**
   * Make unsigned
   */
  using type = std::make_unsigned_t<T>;
};
/**
 * MakeUnsigned works same as std::make_unsigned but works with cassian::Vector
 */
template <typename T> struct MakeUnsigned<T, typename T::is_vector> {
  /**
   * Make unsigned
   */
  using type = Vector<std::make_unsigned_t<typename T::value_type>,
                      T::vector_size, T::size_in_memory>;
};
/**
 * cassian::make_unsigned_t works same as std::make_unsigned_t but it works with
 * cassian::Vector
 */
template <typename T> using make_unsigned_t = typename MakeUnsigned<T>::type;

/**
 * Apply equal operator on Vectors component-wise.
 *
 * @param[in] lhs input a.
 * @param[in] rhs input b.
 * @returns output.
 * @tparam OUTPUT_TYPE the type of output.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename OUTPUT_TYPE, typename T, int N, int SIZE_IN_MEMORY = N>
OUTPUT_TYPE equal_component_wise(const Vector<T, N, SIZE_IN_MEMORY> &lhs,
                                 const Vector<T, N, SIZE_IN_MEMORY> &rhs) {
  using scalar_type = cassian::scalar_type_v<OUTPUT_TYPE>;
  OUTPUT_TYPE output;
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = lhs[i] == rhs[i] ? scalar_type(-1) : scalar_type(0);
  }
  return output;
}

/**
 * Apply equal operator on a Vector and scalar component-wise.
 *
 * @param[in] lhs input a.
 * @param[in] rhs input b.
 * @returns output.
 * @tparam OUTPUT_TYPE the type of output.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename OUTPUT_TYPE, typename T, int N, int SIZE_IN_MEMORY = N>
OUTPUT_TYPE equal_component_wise(const T &lhs,
                                 const Vector<T, N, SIZE_IN_MEMORY> &rhs) {
  const Vector<T, N, SIZE_IN_MEMORY> vector_lhs(lhs);
  return equal_component_wise<OUTPUT_TYPE>(vector_lhs, rhs);
}

/**
 * Apply equal operator on a Vector and scalar component-wise.
 *
 * @param[in] lhs input a.
 * @param[in] rhs input b.
 * @returns output.
 * @tparam OUTPUT_TYPE the type of output.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename OUTPUT_TYPE, typename T, int N, int SIZE_IN_MEMORY = N>
OUTPUT_TYPE equal_component_wise(const Vector<T, N, SIZE_IN_MEMORY> &lhs,
                                 const T &rhs) {
  const Vector<T, N, SIZE_IN_MEMORY> vector_rhs(rhs);
  return equal_component_wise<OUTPUT_TYPE>(lhs, vector_rhs);
}

/**
 * Apply not equal operator on Vectors component-wise.
 *
 * @param[in] lhs input a.
 * @param[in] rhs input b.
 * @returns output.
 * @tparam OUTPUT_TYPE the type of output.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename OUTPUT_TYPE, typename T, int N, int SIZE_IN_MEMORY = N>
OUTPUT_TYPE not_equal_component_wise(const Vector<T, N, SIZE_IN_MEMORY> &lhs,
                                     const Vector<T, N, SIZE_IN_MEMORY> &rhs) {
  using scalar_type = cassian::scalar_type_v<OUTPUT_TYPE>;
  OUTPUT_TYPE output;
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = lhs[i] != rhs[i] ? scalar_type(-1) : scalar_type(0);
  }
  return output;
}

/**
 * Apply not equal operator on a Vector and scalar component-wise.
 *
 * @param[in] lhs input a.
 * @param[in] rhs input b.
 * @returns output.
 * @tparam OUTPUT_TYPE the type of output.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename OUTPUT_TYPE, typename T, int N, int SIZE_IN_MEMORY = N>
OUTPUT_TYPE not_equal_component_wise(const T &lhs,
                                     const Vector<T, N, SIZE_IN_MEMORY> &rhs) {
  const Vector<T, N, SIZE_IN_MEMORY> vector_lhs(lhs);
  return not_equal_component_wise<OUTPUT_TYPE>(vector_lhs, rhs);
}

/**
 * Apply not equal operator on a Vector and scalar component-wise.
 *
 * @param[in] lhs input a.
 * @param[in] rhs input b.
 * @returns output.
 * @tparam OUTPUT_TYPE the type of output.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename OUTPUT_TYPE, typename T, int N, int SIZE_IN_MEMORY = N>
OUTPUT_TYPE not_equal_component_wise(const Vector<T, N, SIZE_IN_MEMORY> &lhs,
                                     const T &rhs) {
  const Vector<T, N, SIZE_IN_MEMORY> vector_rhs(rhs);
  return not_equal_component_wise<OUTPUT_TYPE>(lhs, vector_rhs);
}

/**
 * Apply not operator on a Vector component-wise.
 *
 * @param[in] v input.
 * @returns output.
 * @tparam OUTPUT_TYPE the type of output.
 * @tparam T the type of vector elements.
 * @tparam N the number of vector elements.
 * @tparam SIZE_IN_MEMORY size of vector in memory in elements.
 */
template <typename OUTPUT_TYPE, typename T, int N, int SIZE_IN_MEMORY = N>
OUTPUT_TYPE not_component_wise(const Vector<T, N, SIZE_IN_MEMORY> &v) {
  using scalar_type = cassian::scalar_type_v<OUTPUT_TYPE>;
  OUTPUT_TYPE output;
  for (size_t i = 0; i < output.size(); ++i) {
    output[i] = !v[i] ? scalar_type(-1) : scalar_type(0);
  }
  return output;
}

/**
 * Get vector size. For scalar types return 1.
 *
 * @tparam TYPE type to extract vector size
 * @tparam N disables function when TYPE::vector_size doesn't exist
 * @returns vector size
 *
 */
template <typename TYPE, int N = TYPE::vector_size> int get_vector_size() {
  return N;
}

/**
 * @overload for scalar types
 *
 * @tparam TYPE type to extract vector size
 * @returns 1
 *
 */
template <typename TYPE,
          typename std::enable_if_t<std::is_integral_v<TYPE>, int> = 0>
int get_vector_size() {
  return 1;
}

} // namespace cassian
#endif
