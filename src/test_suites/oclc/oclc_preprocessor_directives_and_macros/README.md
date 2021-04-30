# oclc_preprocessor_directives_and_macros

## Overview
Test suite for 
[preprocessor directives and macros](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#preprocessor-directives-and-macros) 
OpenCL C functionality.

## Tests

### `__FILE__`
* Status: Done
* Goal: Verify that `__FILE__` macro returns proper string literal.
* Description:
  * Mock __FILE__ of .cpp file and pass it to the kernel source.
  * Using #line directive change __FILE__ and write it to the buffer.
* Expectations: Output buffer is compared against reference value computed on the host.

### `__OPENCL_C_VERSION__`
* Status: Done
* Goal: Verify that `__OPENCL_C_VERSION__` macro returns a valid number modified by the values of build options -cl-std=CLX.Y.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against reference value computed on the host.

### `__OPENCL_VERSION__`
* Status: TODO
* Goal: Verify that `__OPENCL_VERSION__` macro returns a valid number.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against reference value computed on the host.

### `CL_VERSION_1_0`
* Status: TODO
* Goal: Verify that `CL_VERSION_1_0` macro returns a valid number.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against reference value computed on the host.

### `CL_VERSION_1_1`
* Status: TODO
* Goal: Verify that `CL_VERSION_1_1` macro returns a valid number.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against reference value computed on the host.

### `CL_VERSION_1_2`
* Status: TODO
* Goal: Verify that `CL_VERSION_1_2` macro returns a valid number.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against reference value computed on the host.

### `CL_VERSION_2_0`
* Status: TODO
* Goal: Verify that `CL_VERSION_2_0` macro returns a valid number.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against reference value computed on the host.

### `__ENDIAN_LITTLE__`
* Status: TODO
* Goal: Verify that `__ENDIAN_LITTLE__` macro returns a valid number for a specific device.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against the reference buffer e.i. query to the current device.

### `__kernel_exec(X, typen) (and kernel_exec(X, typen))`
* Status: TODO
* Goal: Verify that `__kernel_exec(X, typen) (and kernel_exec(X, typen))` macro develops to its definition.
* Description: TODO
* Expectations: TODO

### `__IMAGE_SUPPORT__`
* Status: TODO
* Goal: Verify that `__IMAGE_SUPPORT__` macro returns a valid number for a specific device.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against the reference buffer i.e. query to the current device.

### `__FAST_RELAXED_MATH__`
* Status: TODO
* Goal: Verify that `__FAST_RELAXED_MATH__` macro returns a valid number when `-cl-fast-relaxed-math` optimization option is enabled or not.
* Description: Run OpenCL C kernel that assigns the value of macro to the integer buffer.
* Expectations: Output buffer is compared against the reference value.

### `pragma extensions`
* Status: TODO
* Goal: Verify that all [khronos](https://www.khronos.org/registry/OpenCL/specs/3.0-unified/pdf/OpenCL_Ext.pdf) enabled by pragma extensions.
* Description: If given device supports given extension, check whether pragma extension enables it.
* Expectations: Kernel is being compiled without any errors.
* Parameters:
  * cl_khr_int64_base_atomics
  * cl_khr_int64_extended_atomics
  * TODO

### OpenCL C 3.0 Features
* Status: TODO
* Goal: Verify optional [features](https://www.khronos.org/registry/OpenCL/specs/3.0-unified/html/OpenCL_C.html#features)
* Description: TODO
* Expectations: TODO