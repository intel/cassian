# offline_compiler

## Overview

Wrapper around `ocloc`, the Intel offline compiler. Used to produce SPIR-V (or other
ocloc outputs) ahead of a run, instead of letting the driver JIT-compile kernel
source.

`ocloc` is loaded as a shared library through [system](../system), so the compiler is
an optional runtime dependency — a missing `ocloc` surfaces as an exception rather
than a link error.

## API

```cpp
// compile a kernel file, get the path of the generated SPIR-V
auto spv_path = cassian::generate_spirv(ip_version, "kernel.cl", build_options);

// compile in-memory source, get the SPIR-V bytes
auto spv = cassian::generate_spirv_from_source(ip_version, source, build_options);
```

* `generate_spirv(ip_version, path, build_options = "", quiet = false)` — compiles a
  file and returns the output path.
* `generate_spirv_from_source(ip_version, source, build_options = "", quiet = false)` —
  compiles a source string and returns the SPIR-V as `std::vector<uint8_t>`.
* `generate_offline_compiler_products(ip_version, source_bytes, build_options,
  ocloc_cmd, ocloc_options, program_type, quiet)` — the general form: runs an
  arbitrary ocloc command and returns every artifact it produced as
  `std::vector<OclocProduct>` (`name` + `data`).
* `OfflineCompilerException` — thrown when ocloc cannot be loaded or the compilation
  fails. The compiler's own diagnostics are logged unless `quiet` is set.

`ip_version` identifies the target device and normally comes from
`Runtime::get_device_property(DeviceProperty::ip_version)`, so the kernel is compiled
for the device the test is about to run on.
