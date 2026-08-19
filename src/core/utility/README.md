# utility

## Overview

General-purpose helpers shared by the framework and the test suites: file and asset
loading, bit packing, math on scalars and vectors, Catch2 matchers for approximate
comparison, and template metaprogramming for building type lists.

## utility.hpp

File I/O and assets:

* `load_text_file(path)` / `save_text_file(data, path)`
* `load_binary_file(path)` / `save_binary_file(data, path)`
* `get_application_directory()` — directory of the running binary.
* `get_asset(asset_path)` — resolves a path relative to the binary, so kernels and
  test data can be found regardless of the working directory.
* `convert_to_forward_slashes(str)`

Bit packing and formatting:

* `combine_bytes<OUT>(input)` / `split_bytes<OUT>(input)` — merge or split a value
  byte-wise.
* `pack_vector<OUT>(input, ...)` / `unpack_vector<OUT>(input, ...)` — convert between
  a flat `std::vector` and a vector of wider or `Vector<>` elements. Overloads exist
  for both an explicit output element type and a target `Vector<>` type.
* `to_string(value)` — generic, with overloads for `std::array` and `std::vector`.

Scope guards:

* `finally(action)` returning a `FinalAction<A>` that runs `action` on destruction.

## math.hpp

`dot_product`, `cross_product`, `distance`, `length`, `normalize` — each with a
vector overload (`EnableIfIsVector`) and a scalar overload (`EnableIfIsScalar`), so
generic test code can call them for any width.

## comparators.hpp

Tolerance-based result checking:

* `calculate_ulp_distance(result, reference)` — ULP distance between two values.
* `match_results_ulp(result, reference, ulp)` — compare within a ULP budget.
* `match_results_error_value(result, reference, ...)` — compare within an absolute or
  relative error.
* `match_range(result, start, end)` — accept anything in a range, for builtins with
  implementation-defined precision.
* `PrecisionRequirement<T, REFERENCE>` — declares the tolerance for a type.
* `PrecisionComparator` / `UlpComparator` — Catch2 matchers wrapping the above, for
  use with `REQUIRE_THAT`.
* `raw_bits(value)` / `dec_hex_pair(value)` — formatting helpers that make failure
  messages readable for custom float types and vectors.

## matchers.hpp

* `nan_sensitive_eq(lhs, rhs)` — equality where two NaNs compare equal.
* `nan_sensitive_equal(value)` — the Catch2 matcher form
  (`REQUIRE_THAT(result, nan_sensitive_equal(expected))`).

## metaprogramming.hpp

Building the type lists that drive `TEMPLATE_LIST_TEST_CASE`:

* `static_for(integer_sequence, func)` — compile-time loop.
* `CartesianProduct`, `ProductRow`, `ContainerProductRow`, `CLCTypeProductRow` —
  generate every combination of a set of type lists.
* `TupleConcat` / `tuple_concat_t` — concatenate `std::tuple` type lists.
* `TupleZip` / `tuple_zip_t` — pair up two type lists element by element, for
  source/destination pairs such as conversion tests.

## version.hpp

* `get_version()` — the version string baked in at configure time.
* `print_version()` — logs it; called from `main`.
