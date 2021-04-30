# oclc_vector_load_store_functions

## Overview
Test suite for 
[vector data load and store functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#vector-data-load-and-store-functions) 
OpenCL C functionality.

## Tests

### `vload`
* Status: Done
* Goal: Verify that `vload` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `vload` function.
  * Output is compared against reference values computed on the host.
* Expectations: Loaded values are the same as the ones prepared on the host.
* Parameters:
  * Data type: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `double`
  * Vector elements: 2, 3, 4, 8, 16
  * Memory type: `global`, `local`, `private`, `constant`, `generic`

### `vstore`
* Status: Done
* Goal: Verify that `vstore` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `vstore` function.
  * Output is compared against reference values computed on the host.
* Expectations: Stored values are the same as the ones prepared on the host.
* Parameters:
  * Data type: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `double`
  * Vector elements: 2, 3, 4, 8, 16
  * Memory type: `global`, `local`, `private`, `generic`
