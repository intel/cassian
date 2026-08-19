# fp_types

## Overview

Host implementations of the non-standard floating-point types that OpenCL C and the
hardware use: `Half` (fp16), `Bfloat16` and `Tfloat`. Each behaves like a built-in
arithmetic type so test and reference code can be written generically.

## Types

| Header | Type | Aliases | Storage |
| --- | --- | --- | --- |
| `half.hpp` | `Half` | `half` | `uint16_t` |
| `bfloat16.hpp` | `Bfloat16` | `bfloat16` | `uint16_t` |
| `tfloat.hpp` | `Tfloat` | `tfloat` | `uint32_t` |

Every type provides:

* Conversion from `float` (and from any type convertible to `float`) and back.
* Arithmetic and comparison operators, including mixed operations with integral and
  floating-point scalars.
* `encode(bits)` / `decode()` for raw bit access, so tests can pin exact encodings.
* `nan_sensitive_eq()` for comparisons where two NaNs should count as equal.
* `to_string()`, `operator<<`, and a `std::numeric_limits` specialization.
* `isnan()` / `isinf()` in `namespace cassian`, so `using std::isnan;` followed by an
  unqualified call resolves through ADL for both these types and the built-ins.

`Half` additionally has a stochastic-rounding constructor `Half(float v, float random)`.

## type_traits.hpp

Two traits the rest of the framework keys off:

* `is_floating_point<T>` — extends `std::is_floating_point` to the types above, so
  `is_floating_point_v<Half>` is true.
* `is_custom_type<T>` — true for these types, letting generic code distinguish them
  from the built-in scalars.

## math.hpp

* `abs(value)` — `std::abs` for signed types, identity for unsigned ones.
* `flush_to_zero(value)` — returns a signed zero if the value is subnormal, otherwise
  the value unchanged. Used when a device is expected to denormal-flush.
