# oclc_async_copy_functions

## Overview
Test suite for 
[async copy functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#async-copies) 
OpenCL C functionality.

## Tests

### `async_work_group_copy`
* Status: Done
* Goal: Verify that `async_work_group_copy` function works as expected.
* Description: 
  * Copy input from global buffer to temporary local buffer if needed.
  * Run OpenCL C kernel that calls `async_work_group_copy`.
  * Copy temporary local buffer to output global buffer if needed.
  * Output is compared against reference values computed on the host.
* Expectations: Input buffer is copied to output buffer.
* Parameters:
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.
  * Source/destination:
    * `global` to `local`
    * `local` to `global`
    * `global` to `local` to `global`
  * Number of elements per work item: `1`, `2`.

### `async_work_group_strided_copy`
* Status: TODO
* Goal: Verify that `async_work_group_strided_copy` function works as expected.
* Description: 
  * Copy input from global buffer to temporary local buffer if needed.
  * Run OpenCL C kernel that calls `async_work_group_strided_copy`.
  * Copy temporary local buffer to output global buffer if needed.
  * Output is compared against reference values computed on the host.
* Expectations: Input buffer is copied to output buffer.
* Parameters:
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.
  * Source/destination:
    * `global` to `local`
    * `local` to `global`
    * `global` to `local` to `global`
  * Number of elements per work item: `1`, `2`.
  * Stride: `1`, `2`.

### `wait_group_events`
* Status: TODO
* Goal: Verify that `wait_group_events` works as expected.
* Description: 
  * Run OpenCL C kernel that calls `wait_group_events` function.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.

### `prefetch`
* Status: TODO
* Goal: Verify that `prefetch` works as expected.
* Description: 
  * Run OpenCL C kernel that calls `prefetch` function.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
