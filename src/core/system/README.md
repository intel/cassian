# system

## Overview

Thin platform abstraction for loading a shared library at runtime and resolving
symbols from it. This is how the OpenCL and Level Zero loaders are opened without
link-time dependencies on them.

## API

```cpp
auto library = cassian::load_library("libOpenCL.so");
auto *fn = reinterpret_cast<clGetPlatformIDs_t>(
    library->get_function("clGetPlatformIDs"));
```

* `load_library(name)` — returns `std::unique_ptr<Library>`; picks the Linux or
  Windows implementation at build time. Throws `LibraryNotFoundException`.
* `Library::get_function(name)` — returns a raw `void*` for the caller to cast.
  Throws `LibraryFunctionNotFoundException`.

Unloading happens when the `unique_ptr` goes out of scope, so keep the `Library`
alive for as long as any resolved function pointer is in use.

## Implementation

* `src/library_linux.{hpp,cpp}` — `dlopen` / `dlsym`.
* `src/library_windows.{hpp,cpp}` — `LoadLibrary` / `GetProcAddress`.
* `src/factory.cpp` — selects between them.

Only `factory.hpp` and `library.hpp` are public; the per-platform classes are
private to the module.
