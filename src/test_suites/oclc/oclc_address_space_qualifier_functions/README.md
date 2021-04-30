# oclc_address_space_qualifier_functions

## Overview
Test suite for 
[address space qualifier functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#address-space-qualifier-functions) 
OpenCL C functionality.

## Tests

### `to_global`
* Status: Done
* Goal: Verify that `to_global` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `to_global` on a generic pointer to global memory.
  * Output is verified on the host.
* Expectations: `to_global` returns not `NULL` value.
* Parameters:
  * Const: `yes`, `no`.
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.

### `to_local`
* Status: Done
* Goal: Verify that `to_local` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `to_local` on a generic pointer to local memory.
  * Output is verified on the host.
* Expectations: `to_local` returns not `NULL` value.
* Parameters:
  * Const: `yes`, `no`.
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.

### `to_private`
* Status: Done
* Goal: Verify that `to_private` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `to_private` on a generic pointer to private memory.
  * Output is verified on the host.
* Expectations: `to_private` returns not `NULL` value.
* Parameters:
  * Const: `yes`, `no`.
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.

### `get_fence`
* Status: TODO
* Goal: Verify that `get_fence` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_fence` on a variable.
  * Output is compared against reference values computed on the host.
* Expectations: `get_fence` returns valid `cl_mem_fence_flags`.
* Parameters:
  * Const: `yes`, `no`.
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.
  * Address space qualifier: `global`, `local`.
