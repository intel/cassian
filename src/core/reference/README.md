# reference

## Overview

Host reference implementations of OpenCL C builtins whose expected results are easier
to state as code than as a formula. Small module — two headers.

## dp4a.hpp

Reference for the 4-component integer dot product with accumulation
(`cl_khr_integer_dot_product`):

```cpp
auto expected = cassian::dp4a(input_a, input_b, input_c);
```

* `dp4a(vector<Vector<A,4>>, vector<Vector<B,4>>, vector<int32_t>)` — component-wise
  `dot(a, b) + c` per element, accumulated in `int32_t`.
* `dp4a(vector<A>, vector<B>, vector<int32_t>)` — the packed form, where each input
  element carries four sub-byte values.

Both are templated on the operand signedness, so the same call covers the
signed/unsigned/mixed variants of the builtin.

## extended_bit_operations.hpp

Reference for the `cl_intel_bit_instructions` bitfield builtins. Each is templated on
the operand type and mirrors the builtin's specified semantics, including the
undefined-behaviour boundaries (`count == 0` returns 0; out-of-range
`offset`/`count` is undefined).

* `bitfield_extract_signed(base, offset, count)` — extract with sign extension.
* `bitfield_extract_unsigned(base, offset, count)` — extract, zero-filled.
* `bitfield_insert(base, insert, offset, count)` — replace a bit range in `base`.
* `bit_reverse(base)` — reverse all bits of the type.
* `set_bit(value, offset, bit)` — the shared primitive the others build on.

Floating-point types are rejected with a `static_assert`.
