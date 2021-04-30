# oclc_work_group_functions

## Overview
Test suite for 
[work-group functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#work-group-functions)
and [synchronization functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#synchronization-functions)
OpenCL C functionality.

## Tests

### `work_group_all`
* Status: Done
* Goal: Verify that `work_group_all` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `work_group_all` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a work-group are properly synchronized and computed values are correct.
* Parameters:
  * Values:
    * All work-items in all work-groups have the same non-zero `predicate`.
    * All work-items in all work-groups have different non-zero `predicate`.
    * All work-items in all work-groups have zero `predicate`.
    * All work-items in a work-group have the same non-zero `predicate`.
    * All work-items in a work-group have different non-zero `predicate`.
    * All work-items in a work-group have zero `predicate`.
    * Single work-item in a work-group has non-zero `predicate`, others have zero `predicate`.
    * Single work-item in a work-group has zero `predicate`, others have non-zero `predicate`.
    * Random values.

### `work_group_any`
* Status: Done
* Goal: Verify that `work_group_any` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `work_group_any` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a work-group are properly synchronized and computed values are correct.
* Parameters:
  * Values:
    * All work-items in all work-groups have the same non-zero `predicate`.
    * All work-items in all work-groups have different non-zero `predicate`.
    * All work-items in all work-groups have zero `predicate`.
    * All work-items in a work-group have the same non-zero `predicate`.
    * All work-items in a work-group have different non-zero `predicate`.
    * All work-items in a work-group have zero `predicate`.
    * Single work-item in a work-group has non-zero `predicate`, others have zero `predicate`.
    * Single work-item in a work-group has zero `predicate`, others have non-zero `predicate`.
    * Random values.

### `work_group_broadcast`
* Status: Done
* Goal: Verify that `work_group_broadcast` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `work_group_broadcast` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a work-group are properly synchronized and computed values are correct.
* Parameters:
  * Data types: `int`, `uint`, `long`, `ulong`, `float`, `double`.
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * Value from first local work-item in each dimension is broadcasted.
    * Random local ids.
    * Random input values.
    * Random local ids and input values.

### `work_group_reduce`
* Status: TODO
* Goal: Verify that `work_group_reduce` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `work_group_reduce` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a work-group are properly synchronized and computed values are correct.
* Parameters:
  * Operations: `add`, `min`, `max`.
  * Data types: `int`, `uint`, `long`, `ulong`, `float`, `double`.
  * Values:
    * Zero.
    * Min.
    * Max.
    * Random values.
    * Float specific:
      * Lowest.
      * Denormal.
      * Quiet NaN.
      * Signaling NaN.
      * Infinity.

### `work_group_scan_exclusive`
* Status: TODO
* Goal: Verify that `work_group_scan_exclusive` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `work_group_scan_exclusive` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a work-group are properly synchronized and computed values are correct.
* Parameters:
  * Operations: `add`, `min`, `max`.
  * Data types: `int`, `uint`, `long`, `ulong`, `float`, `double`.
  * Values:
    * Zero.
    * Min.
    * Max.
    * Random values.
    * Float specific:
      * Lowest.
      * Denormal.
      * Quiet NaN.
      * Signaling NaN.
      * Infinity.

### `work_group_scan_inclusive`
* Status: TODO
* Goal: Verify that `work_group_scan_inclusive` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `work_group_scan_inclusive` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a work-group are properly synchronized and computed values are correct.
* Parameters:
  * Operations: `add`, `min`, `max`.
  * Data types: `int`, `uint`, `long`, `ulong`, `float`, `double`.
  * Values:
    * Zero.
    * Min.
    * Max.
    * Random values.
    * Float specific:
      * Lowest.
      * Denormal.
      * Quiet NaN.
      * Signaling NaN.
      * Infinity.


### `work_group_barrier`
* Status: Done
* Goal: Verify that `work_group_barrier` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `work_group_barrier` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a work-group are properly synchronized and computed values are correct.
* Parameters:
  * Types: scalar, TODO: vector, image.
  * Memory type: local, global, TODO: image.
  * Memory scope: work_group, device, TODO: all_svm_devices.
  * Values:
    * Specific value.
    * TODO: Random values.
