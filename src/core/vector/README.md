# vector

## Overview

`Vector<T, N, SIZE_IN_MEMORY>` — a fixed-size host vector type mirroring OpenCL C
vector semantics, plus the type traits that let the rest of the framework write code
that is generic over "scalar or vector".

`SIZE_IN_MEMORY` defaults to `N` and exists for 3-element vectors, which occupy 4
elements in memory just as `float3` does on the device.

## Vector<T, N>

* Construction from an initializer list, a `std::vector`, or a single scalar
  (broadcast); explicit conversion to `Vector<U, N>`.
* Element access via `operator[]`, `size()` and `begin()`; out-of-range access throws
  `VectorAccessViolationException`, and a wrong-length initializer list throws
  `VectorBadNumberOfElementsException`.
* Arithmetic `+ - * / %` with compound assignment, unary minus, and pre/post
  increment and decrement — each available vector-to-vector and vector-to-scalar in
  both operand orders.
* Comparisons `== != < > <= >=`, likewise against both a vector and a scalar.
* `value_type`, and the `is_vector` marker used by the traits below.
* `to_string()` and `operator<<`.

## Traits and helpers

Used throughout the framework to dispatch between scalar and vector code paths:

* `is_vector_v<T>`, `EnableIfIsVector<T>`, `EnableIfIsScalar<T>`,
  `EnableIfIsIntegral<T>`, `EnableIfIsFloatingPoint<T>`.
* `scalar_type_v<T>` — the element type of a vector, or `T` itself for a scalar.
* `make_signed_t<T>` / `make_unsigned_t<T>` — signedness change that also works on
  vectors.
* `get_vector_size<T>()` — `N` for a vector, `1` for an integral scalar.

## Component-wise comparison

OpenCL C relational operators return an integer mask (`-1` for true) rather than a
`bool`. These helpers reproduce that:

`component_wise_compare`, `equal_component_wise`, `not_equal_component_wise`,
`not_component_wise` — each templated on the desired `OUTPUT_TYPE`.
