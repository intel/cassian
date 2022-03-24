# oclc_sub_group_functions

## Overview
Test suite for
[Intel sub group functions](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_subgroups.html)
OpenCL C functionality and specializations for specific data types:
* [char data type](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_subgroups_char.html)
* [short data type](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_subgroups_short.html)
* [long data type](https://www.khronos.org/registry/OpenCL/extensions/intel/cl_intel_subgroups_long.html)

## Tests

### `sub_group_size`
* Status: Done
* Goal: Verify that `sub_group_size` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `sub_group_size` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a subgroup return correct value for subgroup size each time.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * All work-items in all subgroups return correct (equal to last subgroup id) subgroup size. 
    * Subgroup size is between 0 and get_max_sub_group_size.

### `sub_group_broadcast`
* Status: Done
* Goal: Verify that `sub_group_broadcast` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `sub_group_broadcast` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
* Parameters:
  * Data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * Each work item register its own value as subgroup id to be broadcasted. In the same time calls to
    save results of bradcasting in results table from each work item in the subgrups. Finally each
    workitem checks if in results table are correct values (broad casting happened for each work item)

