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

### `get_sub_group_id`
* Status: Done
* Goal: Verify that `get_sub_group_id` `get_sub_group_local_id` `get_sub_group_size` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_sub_group_id` `get_sub_group_local_id` `get_sub_group_size` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a subgroup return correct value for each function each time.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * All work-items in all subgroups return correct value
      * `get_sub_group_local_id` - values from 0 to max subgroup local id
      * `get_sub_group_id` - use value return by this function to build offset in array. Check if in all offsets is true value
      * `get_sub_group_size` - values not bigger than max subgroup local id

### `get_num_sub_groups`
* Status: Done
* Goal: Verify that `get_num_sub_groups` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_num_sub_groups` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a subgroup return correct value for each function each time.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * All work-items in all subgroups return correct (equal to max subgroup id) number of subgroups in workgroup. 

### `get_enqueued_num_sub_groups`
* Status: Done
* Goal: Verify that `get_enqueued_num_sub_groups` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_enqueued_num_sub_groups` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a subgroup return correct value for each function each time.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * All work-items in all subgroups return correct (equal to get_num_sub_groups function) number of enqueued subgroups in workgroup. 

### `get_sub_group_local_id`
* Status: Done
* Goal: Verify that `get_sub_group_local_id` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_sub_group_local_id` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a subgroup return correct value for each function each time.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * All work-items in all subgroups return correct (equal to integer numbers) number of subgroups local ids. 

### `sub_group_shuffle`
* Status: Done
* Goal: Verify that `sub_group_shuffle` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `sub_group_shuffle` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Vector data types (2, 3, 4, 8, 16): `char`, `uchar`, `short`, `ushort`, `float`
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * Each work item register its own value to be shuffle. In the same time calls to
    save results of shuffling in results table from each work item in the subgrups. Finally each
    workitem checks if in results table are correct values (shuffling occure for each work item)
    * In variation of the test `sub_group_shuffle_common_offset` test runs with the same common shuffle index for each work item.

### `sub_group_shuffle_up`
* Status: Done
* Goal: Verify that `sub_group_shuffle_up` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `sub_group_shuffle_up` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Vector data types (2, 3, 4, 8, 16): `char`, `uchar`, `short`, `ushort`, `float`
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * Each work item register its own two values `prev` and `cur` to be shuffle. Which value is shuffled depends on `delta_size` which is the third param of the function. 
    Finally each workitem checks if in results table are correct values (shuffling occure for each work item)
    
### `sub_group_shuffle_down`
* Status: Done
* Goal: Verify that `sub_group_shuffle_down` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `sub_group_shuffle_down` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Vector data types (2, 3, 4, 8, 16): `char`, `uchar`, `short`, `ushort`, `float`
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * Each work item register its own two values `cur` and `next` to be shuffle. Which value is shuffled depends on `delta_size` which is the third param of the function. 
    Finally each workitem checks if in results table are correct values (shuffling occure for each work item)

### `sub_group_shuffle_xor`
* Status: Done
* Goal: Verify that `sub_group_shuffle_xor` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `sub_group_shuffle_xor` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Vector data types (2, 3, 4, 8, 16): `char`, `uchar`, `short`, `ushort`, `float`
  * Dimensions: 1D, 2D, 3D.
  * Values:
    * Each work item register its value `data`. Which value is shuffled depends on `mask` param which is the second param of the function. 
    Finally each workitem checks if in results table are correct values (shuffling occure for each work item)