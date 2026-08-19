# test_harness

## Overview

Test requirements and shared test configuration. A test declares what it needs from
the device; the harness decides whether the test can run and skips it with a logged
reason when it cannot.

## Requirements

`test_harness.hpp` — build up a `Requirements` object, then ask whether to skip:

```cpp
cassian::Requirements requirements;
requirements.feature(cassian::Feature::fp64);
requirements.arithmetic_type<TestType>();
requirements.min_work_group_size(16);
if (cassian::should_skip_test(requirements, *runtime)) {
  return;
}
```

* `feature(Feature)` — an optional device capability.
* `property(unique_ptr<PropertyCheck>)` — any custom check; the generic escape hatch.
* `arithmetic_type<T>()` — the feature implied by a type (e.g. `half` requires
  `fp16`), so template test cases declare their requirement automatically.
* `atomic_type<T>()`, `atomic_add<T>()`, `atomic_min_max<T>()`,
  `atomic_load_store<T>()` — atomic capabilities, scoped by `AtomicMemoryType`
  (`local`, `global`, `all`).
* `correctly_rounded_divide_sqrt<T>()`

The type-parametrized methods above use the same pattern: the primary template is an
empty no-op, and explicit specializations add a requirement only for the types that
actually need one (`atomic_type<clc_long_t>`, `atomic_add<clc_half_t>`, and so on).
Calling them for a type with no specialization is intentionally free, so template test
cases can declare them unconditionally.
* `min_work_group_size(x, y = 1, z = 1)`
* `sub_group_size<N>()` — specialized for 16 and 32.
* `openclc_feature(feature, program_type)`
* `should_skip_test(requirements, runtime)` — evaluates every collected check and
  returns true if any is unmet, logging which one.

## TestConfigBase

`test_config.hpp` — the per-run configuration shared by all test cases:

* `runtime()` — the `Runtime *` every test uses.
* `program_type()` — the kernel language / program type for this run.
* `add_harness_arguments(parser)` — registers the corresponding command-line
  arguments.

`TestConfigBase` is constructed from a `CommandLineParser` and is non-copyable; it
owns the `Runtime`. Test binaries derive from it — see `Config` in [main](../main) —
so a project can add its own settings while reusing the runtime and program-type
handling.
