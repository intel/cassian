# main

## Overview

The test binary entry point plus the conveniences that make a test case short: a
global config, a build-flags builder, and a helper that hides the
create-buffer / set-argument / run / read-back sequence.

Link this module into a test suite and you get a `main()`; you do not write one.

## main()

`src/main.cpp` is the whole program:

1. `print_version()`
2. build a `CommandLineParser`, let `add_harness_arguments()` register the framework
   arguments, and parse — the recognized arguments are removed from `argv`
3. construct a `Config` from the parsed arguments and publish it with `set_config()`
4. hand the remaining arguments to `Catch::Session().run()`

So framework arguments and Catch2 arguments can be mixed freely on the command line.

## Config

`config.hpp` — `Config` derives from `TestConfigBase` (see
[test_harness](../test_harness)) and is constructed from the parser.
`get_config()` / `set_config()` give test cases access to it; in practice tests reach
the runtime through `test::runtime()` below.

## FlagsBuilder

`flags_builder.hpp` — assemble compiler flags without string juggling:

```cpp
const auto flags = cassian::test::FlagsBuilder(Language::opencl_c)
                       .define("SIZE", std::to_string(size))
                       .include(dir)
                       .optimize(OptimizationLevel::none)
                       .str();
```

Constructed with a `Language` (`opencl_c` or `cm`, which seeds the language-specific
base flags), then chained: `append()`, `define(macro)` / `define(macro, value)`,
`include()`, `optimize(OptimizationLevel)`, `debug()`, and `str()` to finish.

## Test helper

`test_helper.hpp` — free functions in `cassian::test` that wrap a singleton `Helper`
holding the kernel, its arguments and the resources to clean up:

```cpp
using namespace cassian::test;

input(input_data);          // create a buffer, fill it, pass it as the next argument
output(result, size);       // create a buffer, pass it, read it back after execution
pass(scalar);               // pass a scalar argument
kernel(global_work_size, "kernel_name", source, flags);
```

* `runtime()`, `default_program_type()` — access the current config.
* `pass(arg)` — append a scalar, `Buffer`, `Image` or `Sampler` argument.
* `input()`, `output()`, `input_output()` — create a buffer, wire it up as the next
  argument, and (for the output forms) read it back once the kernel has run.
* `sampler()` — create and pass a sampler.
* `kernel(...)` — 17 overloads: with or without an explicit global size (1D/2D/3D or a
  bare `size_t`), with or without a local size, from a single source or from a list of
  `ProgramDescriptor`s. Compiles, sets the collected arguments, runs, performs the
  registered read-backs, and cleans up.
* `add_action_after_exec(action)` — run something after the kernel completes but before
  cleanup.

Image and NV12 overloads exist for the same pattern, templated on the pixel type.
