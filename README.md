# Cassian
Cassian is a multi-API conformance test suite for compute kernel languages.

Main goals:
* Provide a framework for developing black-box tests for compute kernels.
* Abstract runtime layer so the same kernel can be consumed by different APIs.

Currently supported:
* Languages:
  * OpenCL C
  * C for Metal
* Runtimes:
  * OpenCL
  * oneAPI Level Zero

## Getting started

### Build dependencies
1. [Catch2](https://github.com/catchorg/Catch2)
1. [OpenCL Headers](https://github.com/KhronosGroup/OpenCL-Headers)
1. [oneAPI Level Zero Headers](https://github.com/oneapi-src/level-zero)

### Runtime dependencies
1. OpenCL driver
1. oneAPI Level Zero driver
1. OpenCL offline compiler (ocloc)

All above runtime dependencies can be found in [compute-runtime](https://github.com/intel/compute-runtime/releases) project.

### Build
    git clone https://github.com/intel/cassian
    cd cassian
    cmake -B build -S .
    cmake --build build --parallel

## Alternatives
The following list contains projects that at first sight looks similar to Cassian and the explanation how Cassian is different from them:
1. [OpenCL CTS](https://github.com/KhronosGroup/OpenCL-CTS) - tests for OpenCL API and OpenCL C. Cassian focuses mainly on kernel languages including OpenCL C and on support for multiple APIs like OpenCL and Level Zero.
1. [oneAPI Level Zero Tests](https://github.com/oneapi-src/level-zero-tests) - tests for Level Zero API layer. Cassian supplements this project by providing tests for kernel language features using Level Zero API.
1. [Amber](https://github.com/google/amber) - multi-API test suite for shader languages. Main concepts of Amber and Cassian are similar i.e. test multiple kernel/shader languages using different runtime APIs. Amber focuses on 3D APIs and shader languages where Cassian targets Compute environments.

## License
Cassian is licensed under the [MIT License](LICENSE).

Catch2 extensions in [catch2_utils.hpp](src/core/catch2_utils/include/cassian/catch2_utils/catch2_utils.hpp) are licensed under the [Boost Software License](src/core/catch2_utils/include/cassian/catch2_utils/LICENSE_1_0.txt).
