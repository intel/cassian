# runtime

## Overview

The API abstraction layer at the heart of Cassian: one `Runtime` interface with an
OpenCL and a Level Zero implementation, so the same test and the same kernel can be
run through either API.

Tests never name a concrete backend. They receive a `Runtime *` from the test config
and call the virtual interface; the backend is chosen by a command-line argument.

## Runtime interface

`runtime.hpp` declares the handle types and the interface.

Handles — plain value types wrapping an opaque id: `Buffer`, `Image`, `Sampler`,
`Kernel`, `LocalMemory`.

`Runtime` (abstract):

* Lifecycle — `initialize()`, `initialize_subdevices()`, `get_subdevice()`,
  `get_subdevice_count()`.
* Resources — `create_buffer()`, `create_image()`, `get_image_plane()`,
  `create_sampler()`, and the matching `release_*()`. Each takes an
  `AccessQualifier`.
* Data movement — `read_buffer()` / `write_buffer()` / `read_image()` /
  `write_image()` for raw pointers, and the typed convenience templates
  `read_buffer_to_vector<T>()` / `write_buffer_from_vector<T>()`, specialized for the
  custom `fp_types` types.
* Kernels — `create_kernel()` from source, `create_kernel_from_multiple_programs()`
  for multi-module linking, `create_program_and_get_native_binary()`.
* Dispatch — `set_kernel_argument()` overloads (scalar, `Buffer`, `Image`, `Sampler`,
  `LocalMemory`) and a family of `run_kernel()` overloads covering 1D/2D/3D global
  sizes, optional local size, and an explicit device index.
* Capabilities — `is_feature_supported(Feature)`, `get_device_property(DeviceProperty)`.

Errors are reported as `RuntimeException`.

## Descriptors and enums

* `feature.hpp` — `Feature`: optional capabilities a test may require (`fp16`, `fp64`,
  images, sampling, and the per-type/per-scope atomic capabilities), plus
  `to_string`.
* `device_properties.hpp` — `DeviceProperty`: queryable integers such as
  `max_total_group_size`, `max_local_memory_size`, `simd_width`, `ip_version`,
  `device_id`.
* `access_qualifier.hpp` — `AccessQualifier`: `read_write`, `read_only`, `write_only`.
* `image_properties.hpp` — `ImageDimensions`, `ImageType`, `ImageFormat`,
  `ImageChannelOrder`, `ImagePlane`, `get_pixel_size()`.
* `sampler_properties.hpp` — `SamplerCoordinates`, `SamplerAddressingMode`,
  `SamplerFilterMode`.
* `program_descriptor.hpp` — `ProgramDescriptor`: one program in a multi-program link.

## Type descriptors

* `openclc_types.hpp` — `clc_*_t` descriptors mapping an OpenCL C type to its host
  type, device spelling and abbreviation, for every scalar and vector width.
* `openclc_type_tuples.hpp` — ready-made `std::tuple` type lists built from those
  descriptors, for use with `TEMPLATE_LIST_TEST_CASE`. By width:
  `ScalarTypes`, `Vector2Types` … `Vector16Types`, `VectorTypes`, each with
  `*IntegerTypes` and `*FloatingPointTypes` variants; by element:
  `TypesChar` … `TypesDouble`, `SignedTypes`, `UnsignedTypes`, `IntegerTypes`; and
  source/destination pairs for conversion tests, `ScalarToScalar`,
  `ScalarToVector2`, `Vector4ToVector16`, and so on for every combination.

## Feature probing and requirements

* `openclc_utils.hpp` — `check_optional_openclc_macro()`,
  `check_optional_openclc_feature_support()`: trial-compile a probe to find out
  whether a language feature exists.
* `cm_utils.hpp` — the same for C for Metal.
* `property_checks.hpp` — `PropertyCheck` base plus `MinWorkGroupSize` and
  `OpenclcFeature`. These are what a test's `Requirements` collects; see
  [test_harness](../test_harness).

## Factory

`factory.hpp`:

* `create_runtime(name)` — `"ocl"` or `"l0"`; throws `UnknownRuntimeException`.
* `create_runtime_extra(name)` — an extension hook. The default implementation
  returns `nullptr`; a downstream project can substitute its own by pointing the
  `CASSIAN_RUNTIME_FACTORY_EXTRA` CMake variable at a replacement source file, which
  lets it add backends without modifying this module.
* `add_runtime_arguments(parser)` — registers the runtime-selection arguments.

## Backends and mocks

Private to the module: `src/opencl_runtime.*` and `src/level_zero_runtime.*` are the
implementations; `src/opencl_wrapper.*` and `src/level_zero_wrapper.*` load the
respective loader library via [system](../system) and resolve its entry points, so
neither API is a link-time dependency.

Two mocks are available for unit-testing framework code without a device:
`mocks/dummy_runtime.hpp` implements every virtual method as a no-op, and
`mocks/stub_runtime.hpp` derives from it and exposes public fields
(`is_fp16_supported_`, `max_group_size_x_`, …) that a test sets to control what the
capability and property queries report.

## Adding a language or API

* Consuming a new kernel language through an existing API: add a `program_type` and
  handle it in `create_kernel()`.
* Adding a whole API: implement the `Runtime` interface, add a wrapper that loads its
  loader library, and register the name in `create_runtime()`.
