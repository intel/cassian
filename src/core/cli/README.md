# cli

## Overview

Minimal command-line argument parser used by the test binaries. Arguments are
registered up front with a default value, parsed out of `argv`, and read back by
name and type.

## API

`CommandLineParser`:

* `add_argument(name, default_value = "")` — register an argument.
* `parse(&argc, argv)` — consume the recognized arguments and remove them from
  `argv`, so the remaining arguments can be handed to Catch2.
* `get<T>(name)` — value converted by `T(std::string)`. `std::string` and any type
  constructible from a string work directly.
* `list_requested()` — true if the caller asked for a listing rather than a run.

## Example

```cpp
cassian::CommandLineParser parser;
parser.add_argument("--runtime", "ocl");
parser.parse(&argc, argv);
const auto runtime_name = parser.get<std::string>("--runtime");
```

Callers normally do not construct the parser themselves: `main` does it and passes
the result into `Config`. Modules that need their own arguments register them by
providing an `add_*_arguments(CommandLineParser*)` function — see
`add_runtime_arguments` in [runtime](../runtime) and `add_harness_arguments` in
[test_harness](../test_harness).
