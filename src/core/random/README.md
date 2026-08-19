# random

## Overview

Seeded random value generation for test inputs. Two entry points —
`generate_value<T>()` for one value and `generate_vector<T>()` for a whole buffer —
each covering scalars, `Vector<>` types and the `fp_types` custom types.

## API

```cpp
// single scalar in [min, max]
auto v = cassian::generate_value<int>(0, 100, seed);

// vector type: min/max are given as the scalar element type
auto v4 = cassian::generate_value<cassian::Vector<float, 4>>(0.0F, 1.0F, seed);

// exclude specific values (e.g. avoid a divide by zero)
auto nonzero = cassian::generate_value<int>(-10, 10, seed, {0});

// a buffer's worth of input data
auto input = cassian::generate_vector<float>(size, 0.0F, 1.0F, seed);
```

* `generate_value<T>(min, max, seed)` and `generate_value<T>(min, max, seed, except)` —
  scalar overloads via `EnableIfIsScalar`; the `except` form re-draws until the value
  is not in the list.
* The same two signatures via `EnableIfIsVector` for vector types, with `min`/`max`
  taken as `scalar_type_v<T>` and applied per component.
* Explicit specializations for `int8_t`, `uint8_t`, `float`, `double`, `half`,
  `bfloat16` and `tfloat` — the generic form relies on
  `std::uniform_int_distribution`, which does not accept those element types.
* `generate_vector<T>(size, min, max, seed)` — fills a `std::vector<T>`; also a
  `generate_vector<T>(size, seed)` form for types with an implied range.
* `generate_random_quiet_nan(seed)` — a random quiet NaN bit pattern, for NaN
  propagation tests.

## Seeding behaviour

Each generator holds its engine in a function-local `static`:

```cpp
static std::default_random_engine engine(seed);
```

The engine is therefore constructed — and the `seed` argument honoured — on the
**first** call for a given instantiation; later calls advance that same engine and
ignore the `seed` they are passed. A sequence is reproducible across runs of the same
binary, but you cannot re-seed mid-run by passing a different value.
