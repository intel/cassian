# oclc_relational_functions

## Overview
Test suite for 
[relational functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#relational-functions) 
OpenCL C functionality.

## Tests

### `isequal`
* Status: Done
* Goal: Verify that `isequal` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isequal` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are equal to y.
    * All components of x are not equal to y.
    * Some components of x are equal to y, others are not.
    * All components of x and y are positive zero.
    * All components of x and y are negative zero.
    * All components of x are positive zero, and all components of y are negative zero.
    * All components of x and y are quiet NaN.
    * All components of x are quiet NaN, all components of y are normal.
    * All components of x are quiet NaN, all components of y are infinity.
    * All components of x and y are signaling NaN.
    * All components of x are signaling NaN, all components of y are normal.
    * All components of x are signaling NaN, all components of y are infinity.
    * All components of x and y are infinity.
    * All components of x are infinity, all components of y are normal.
    * All components of x and y are denormal.
    * All components of x and y differ by epsilon.
    * Random values.

### `isnotequal`
* Status: Done
* Goal: Verify that `isnotequal` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isnotequal` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are equal to y.
    * All components of x are not equal to y.
    * Some components of x are equal to y, others are not.
    * All components of x and y are positive zero.
    * All components of x and y are negative zero.
    * All components of x are positive zero, and all components of y are negative zero.
    * All components of x and y are quiet NaN.
    * All components of x are quiet NaN, all components of y are normal.
    * All components of x are quiet NaN, all components of y are infinity.
    * All components of x and y are signaling NaN.
    * All components of x are signaling NaN, all components of y are normal.
    * All components of x are signaling NaN, all components of y are infinity.
    * All components of x and y are infinity.
    * All components of x are infinity, all components of y are normal.
    * All components of x and y are denormal.
    * All components of x and y differ by epsilon.
    * Random values.

### `isgreater`
* Status: TODO
* Goal: Verify that `isgreater` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isgreater` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are greater than y.
    * All components of x are not greater than y.
    * Some components of x are greater than y, others are not.
    * All components of x and y are positive zero.
    * All components of x and y are negative zero.
    * All components of x are positive zero, all components of y are negative zero.
    * All components of x are NaN.
    * All components of y are NaN.
    * All components of x and y are NaN.
    * All components of x are positive infinity.
    * All components of x are negative infinity.
    * All components of y are positive infinity.
    * All components of y are negative infinity.
    * All components of x and y are positive infinity.
    * All components of x and y are negative infinity.
    * Random values.

### `isgreaterequal`
* Status: TODO
* Goal: Verify that `isgreaterequal` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isgreaterequal` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are greater than y.
    * All components of x are equal to y.
    * All components of x are less than y.
    * Some components of x are greater than y, others are equal.
    * Some components of x are less than y, others are equal.
    * Some components of x are greater than y, others are less.
    * All components of x and y are positive zero.
    * All components of x and y are negative zero.
    * All components of x are positive zero, all components of y are negative zero.
    * All components of x are NaN.
    * All components of y are NaN.
    * All components of x and y are NaN.
    * All components of x are positive infinity.
    * All components of x are negative infinity.
    * All components of y are positive infinity.
    * All components of y are negative infinity.
    * All components of x and y are positive infinity.
    * All components of x and y are negative infinity.
    * Random values.

### `isless`
* Status: TODO
* Goal: Verify that `isless` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isless` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are less than y.
    * All components of x are not less than y.
    * Some components of x are less than y, others are not.
    * All components of x and y are positive zero.
    * All components of x and y are negative zero.
    * All components of x are positive zero, all components of y are negative zero.
    * All components of x are NaN.
    * All components of y are NaN.
    * All components of x and y are NaN.
    * All components of x are positive infinity.
    * All components of x are negative infinity.
    * All components of y are positive infinity.
    * All components of y are negative infinity.
    * All components of x and y are positive infinity.
    * All components of x and y are negative infinity.
    * Random values.

### `islessequal`
* Status: TODO
* Goal: Verify that `islessequal` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `islessequal` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are greater than y.
    * All components of x are equal to y.
    * All components of x are less than y.
    * Some components of x are greater than y, others are equal.
    * Some components of x are less than y, others are equal.
    * Some components of x are greater than y, others are less.
    * All components of x and y are positive zero.
    * All components of x and y are negative zero.
    * All components of x are positive zero, all components of y are negative zero.
    * All components of x are NaN.
    * All components of y are NaN.
    * All components of x and y are NaN.
    * All components of x are positive infinity.
    * All components of x are negative infinity.
    * All components of y are positive infinity.
    * All components of y are negative infinity.
    * All components of x and y are positive infinity.
    * All components of x and y are negative infinity.
    * Random values.

### `islessgreater`
* Status: TODO
* Goal: Verify that `islessgreater` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `islessgreater` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are greater than y.
    * All components of x are equal to y.
    * All components of x are less than y.
    * Some components of x are greater than y, others are equal.
    * Some components of x are less than y, others are equal.
    * Some components of x are greater than y, others are less.
    * All components of x and y are positive zero.
    * All components of x and y are negative zero.
    * All components of x are positive zero, all components of y are negative zero.
    * All components of x are NaN.
    * All components of y are NaN.
    * All components of x and y are NaN.
    * All components of x are positive infinity.
    * All components of x are negative infinity.
    * All components of y are positive infinity.
    * All components of y are negative infinity.
    * All components of x and y are positive infinity.
    * All components of x and y are negative infinity.
    * Random values.

### `isfinite`
* Status: TODO
* Goal: Verify that `isfinite` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isfinite` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components are finite.
    * All components are positive infinity.
    * All components are negative infinity.
    * Some components are finite, others are positive infinity.
    * Some components are finite, others are negative infinity.
    * All components are NaN.
    * Some components are NaN.
    * Random values.

### `isinf`
* Status: TODO
* Goal: Verify that `isinf` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isinf` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components are finite.
    * All components are positive infinity.
    * All components are negative infinity.
    * Some components are finite, others are positive infinity.
    * Some components are finite, others are negative infinity.
    * All components are NaN.
    * Some components are NaN.
    * Random values.

### `isnan`
* Status: TODO
* Goal: Verify that `isnan` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isnan` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components are NaN.
    * Some components are NaN.
    * All components are not NaN.
    * All components are positive infinity.
    * All components are negative infinity.
    * Random values.

### `isnormal`
* Status: TODO
* Goal: Verify that `isnormal` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isnormal` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components are normal.
    * Some components are normal, others are denormal.
    * All components are denormal.
    * All components are positive infinity.
    * All components are negative infinity.
    * All components are NaN.
    * Random values.

### `isordered`
* Status: TODO
* Goal: Verify that `isorderer` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isorderer` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are NaN, y is ordered.
    * All components of y are NaN, x is ordered.
    * All components of x and y are NaN.
    * Some components of x are NaN, others are normal, y is ordered.
    * Some components of y are NaN, others are normal, x is ordered.
    * None components of x or y are NaN.
    * Random values.

### `isunordered`
* Status: TODO
* Goal: Verify that `isunordered` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `isunordered` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components of x are NaN, y is ordered.
    * All components of y are NaN, x is ordered.
    * All components of x and y are NaN.
    * Some components of x are NaN, others are normal, y is ordered.
    * Some components of y are NaN, others are normal, x is ordered.
    * None components of x or y are NaN.
    * Random values.

### `signbit`
* Status: TODO
* Goal: Verify that `signbit` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `signbit` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `float`, `double`, `floatn`, `doublen`.
  * Values:
    * All components are signed.
    * Some components are signed.
    * All components are unsigned.
    * All components are positive zero.
    * All components are negative zero.
    * All components are NaN.
    * All components are positive infinity.
    * All components are negative infinity.
    * Random values.

### `any`
* Status: TODO
* Goal: Verify that `any` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `any` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`.
  * Values:
    * All bits in all components are set.
    * None bits in all components are set.
    * Not most significant bits in all components are set.
    * Only most significant bit in all components are set.
    * Most significant bit in some components is set.
    * Random values.

### `all`
* Status: TODO
* Goal: Verify that `all` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `all` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`.
  * Values:
    * All bits in all components are set.
    * None bits in all components are set.
    * Not most significant bits in all components are set.
    * Only most significant bit in all components are set.
    * Most significant bit in some components is set.
    * Random values.

### `bitselect`
* Status: TODO
* Goal: Verify that `bitselect` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `bitselect` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.
  * Values:
    * All bits in all components of c are set.
    * All bits in all components of c are not set.
    * Even bits in all components of c are set, odd bits in all components of c are not set.
    * All bits in even components of c are set, all bits in odd components of c are not set.
    * Random values.

### `select`
* Status: Done
* Goal: Verify that `select` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `select` on requested scalar and vector data types.
  * Output is compared against reference values computed on the host.
* Expectations: Computed values are correct.
* Parameters:
  * Data types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`,
                `float`, `floatn`, `double`, `doublen`.
  * Select types: `char`, `charn`, `short`, `shortn`, `int`, `intn`, `long`, `longn`,
                  `uchar`, `ucharn`, `ushort`, `ushortn`, `uint`, `uintn`, `ulong`, `ulongn`.
  * Values:
    * Most significant bit in all components of c are set.
    * Most significant bit in all components of c are not set.
    * Most significant bit in even components of c are set, most significant bit in odd components of c are not set.
    * Random values.
