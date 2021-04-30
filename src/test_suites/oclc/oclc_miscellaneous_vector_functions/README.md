# oclc_miscellaneous_vector_functions

## Overview
Test suite for 
[miscellaneous vector functions functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#miscellaneous-vector-functions) 
OpenCL C functionality.

## Tests

### `vec_step_data_type`
* Status: DONE
* Goal: Verify that `vec_step` function works with a data type as an argument.
* Description: 
  * Run OpenCL C kernel that calls `vec_step` with a variable as an argument.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.

### `vec_step_pure_type`
* Status: DONE
* Goal: Verify that `vec_step` function works with a pure type as an argument.
* Description: 
  * Run OpenCL C kernel that calls `vec_step` with a pure type as an argument.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.

### `shuffle`
* Status: TODO
* Goal: Verify that `shuffle` works as expected.
* Description: 
  * Run OpenCL C kernel that calls `shuffle` function.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### `shuffle2`
* Status: TODO
* Goal: Verify that `shuffle2` works as expected.
* Description: 
  * Run OpenCL C kernel that calls `shuffle2` function.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
