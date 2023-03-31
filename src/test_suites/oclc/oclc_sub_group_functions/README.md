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
* Expectations:
  * All work-items in all subgroups return correct (equal to last subgroup id) subgroup size.
  * Subgroup size is between 0 and get_max_sub_group_size.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `sub_group_broadcast`
* Status: Done
* Goal: Verify that `sub_group_broadcast` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_broadcast` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations:
  * Each work-item register its own value as subgroup id to be broadcasted. In the same time calls to
    save results of broadcasting in results table from each work-item in the subgroups. Finally each
    work-item checks if in results table are correct values (broadcasting happened for each work-item).
* Parameters:
  * Data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `get_sub_group_id`
* Status: Done
* Goal: Verify that `get_sub_group_id` `get_sub_group_local_id` `get_sub_group_size` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `get_sub_group_id` `get_sub_group_local_id` `get_sub_group_size` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations:
  * All work-items in all subgroups return correct value
    * `get_sub_group_local_id` - values from 0 to max subgroup local id.
    * `get_sub_group_id` - use value return by this function to build offset in array. Check if in all offsets is true value.
    * `get_sub_group_size` - values not bigger than max subgroup local id.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_num_sub_groups`
* Status: Done
* Goal: Verify that `get_num_sub_groups` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `get_num_sub_groups` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations:
  * All work-items in all subgroups return correct (equal to max subgroup id) number of subgroups in workgroup.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_enqueued_num_sub_groups`
* Status: Done
* Goal: Verify that `get_enqueued_num_sub_groups` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `get_enqueued_num_sub_groups` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations:
  * All work-items in all subgroups return correct (equal to get_num_sub_groups function) number of enqueued subgroups in workgroup.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_sub_group_local_id`
* Status: Done
* Goal: Verify that `get_sub_group_local_id` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `get_sub_group_local_id` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations:
  * All work-items in all subgroups return correct (equal to integer numbers) number of subgroups local ids.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `sub_group_shuffle`
* Status: Done
* Goal: Verify that `sub_group_shuffle` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_shuffle` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its own value to be shuffle. In the same time calls to
    save results of shuffling in results table from each work-item in the subgroups. Finally each
    work-item checks if in results table are correct values (shuffling occured for each work-item)
    * In variation of the test `sub_group_shuffle_common_offset` test runs with the same common shuffle index for each work-item.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Vector data types (2, 3, 4, 8, 16): `char`, `uchar`, `short`, `ushort`, `float`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_shuffle_up`
* Status: Done
* Goal: Verify that `sub_group_shuffle_up` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_shuffle_up` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its own two values `prev` and `cur` to be shuffle. Which value is shuffled depends on `delta_size` which is the third param of the function.
    Finally each work-item checks if in results table are correct values (shuffling occured for each work-item).
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Vector data types (2, 3, 4, 8, 16): `char`, `uchar`, `short`, `ushort`, `float`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_shuffle_down`
* Status: Done
* Goal: Verify that `sub_group_shuffle_down` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_shuffle_down` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its own two values `cur` and `next` to be shuffle. Which value is shuffled depends on `delta_size` which is the third param of the function.
    Finally each work-item checks if in results table are correct values (shuffling occured for each work-item).
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Vector data types (2, 3, 4, 8, 16): `char`, `uchar`, `short`, `ushort`, `float`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_shuffle_xor`
* Status: Done
* Goal: Verify that `sub_group_shuffle_xor` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_shuffle_xor` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value `data`. Which value is shuffled depends on `mask` param which is the second param of the function.
    Finally each work-item checks if in results table are correct values (shuffling occured for each work-item).
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Vector data types (2, 3, 4, 8, 16): `char`, `uchar`, `short`, `ushort`, `float`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_reduce_min`
* Status: Done
* Goal: Verify that `sub_group_reduce_min` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_reduce_min` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the reduce function. The minimal value `expected_value` among all subgroup work-items should be result.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_reduce_max`
* Status: Done
* Goal: Verify that `sub_group_reduce_max` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_reduce_max` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the reduce function. The maximal value `expected_value` among all subgroup work-items should be result.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_reduce_add`
* Status: Done
* Goal: Verify that `sub_group_reduce_add` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_reduce_add` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the reducing. The maximal value `expected_value` among all subgroup work-items should be result.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_scan_inclusive_min`
* Status: Done
* Goal: Verify that `sub_group_scan_inclusive_min` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_scan_inclusive_min` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the scan function. The minimal value `expected_value` for each work-item should be returned.
    Because minimal value is 1 (value registered for first work-item) test expect 1 for all work-items.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_scan_inclusive_max`
* Status: Done
* Goal: Verify that `sub_group_scan_inclusive_max` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_scan_inclusive_max` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the scan function. The maximal value `expected_value` for each work-item should be returned.
    In this case maximal result value is changing and equals to `sub_group_local_id + 1` (value of current work-item).
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_scan_inclusive_add`
* Status: Done
* Goal: Verify that `sub_group_scan_inclusive_add` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_scan_inclusive_add` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the scan function. The value `expected_value` for each work-item should be returned.
    In this case result value is changing for each work-item and equals the sum of `sub_group_local_id + 1` all
    previous and current work-item.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_scan_exclusive_min`
* Status: Done
* Goal: Verify that `sub_group_scan_exclusive_min` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_scan_exclusive_min` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the scan function. The minimal value `expected_value` for each work-item should be returned.
    Exclusion from calculation means values from all work-items are calculated excluding current which id equals `sub_group_local_id`. That why maximal value for particular data type is expected as initial value for first work-item.
    Others work-items get value 1 as result (coming from first work-item).
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_scan_exclusive_max`
* Status: Done
* Goal: Verify that `sub_group_scan_exclusive_max` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_scan_exclusive_max` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the scan function. The maximal value `expected_value` for each work-item should be returned.
    Exclusion from calculation means values from all work-items are calculated excluding current which id equals `sub_group_local_id`. That why minimal value for particular data type is expected as initial value for first work-item.
    Others work-items get value `sub_group_local_id` as result.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_scan_exclusive_add`
* Status: Done
* Goal: Verify that `sub_group_scan_exclusive_add` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_scan_exclusive_add` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
  * Each work-item register its value to the scan function. The value `expected_value` for each work-item should be returned.
    In this case result value is changing for each work-item and equals the sum of `sub_group_local_id + 1` values for all work-items excluding current which id equals `sub_group_local_id`. For first work-item (no previous work-items) value 0 is returned.
* Parameters:
  * Scalar data types: `char`, `uchar`, `short`, `ushort`, `int`, `uint`, `long`, `ulong`, `float`, `half`, `double`.
  * Dimensions: 1D, 2D, 3D.

### `sub_group_all`
* Status: Done
* Goal: Verify that `sub_group_all` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_all` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
* Test cases:
  * Positive case - test when predicate for all subgroups work-items evaluate to true.
  * Negative case - test when predicate for some subgroups work-items evaluate to false.

### `sub_group_any`
* Status: Done
* Goal: Verify that `sub_group_any` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_any` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Work-items in a sub-group are properly synchronized and computed values are correct.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
* Test cases:
  * Positive case - test when predicate for some subgroups work-items evaluate to true.
  * Negative case - test when predicate for all subgroups work-items evaluate to false.

### `sub_group_block_read`
* Status: Done
* Goal: Verify that `sub_group_block_read` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_block_read` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Blocks of data are read correctly from buffer data types. Results match expected values
* Parameters:
  * Data types: uint, uint2, uint4, uint8, uchar, uchar2, uchar4, uchar8, ushort, ushort2, ushort4, ushort8, ulong, ulong2, ulong4, ulong8
  * Dimensions: 1D, 2D, 3D.
  * Object Types: buffers, images.
* Test case
  * Only read operation in kernel from buffer object.
  * Only read operation in kernel from image object.

### `sub_group_block_write`
* Status: Done
* Goal: Verify that `sub_group_block_write` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_block_write` in multiple work-groups and sub-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Blocks of data are written correctly to buffer data types. Results match expected values
* Parameters:
  * Data types: uint, uint2, uint4, uint8, uchar, uchar2, uchar4, uchar8, ushort, ushort2, ushort4, ushort8, ulong, ulong2, ulong4, ulong8
  * Dimensions: 1D, 2D, 3D.
* Test case
  * Only write operation in kernel to buffer object.
  * Only write operation in kernel from buffer object.
  * Read and Write operations in one kernel from/to buffer object.
  * Read and Write operations in one kernel from/to image object.

### `sub_group_media_block_read`
* Status: Done
* Goal: Verify that `sub_group_media_block_read` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_media_block_read` in multiple work-groups, sub-groups, media blocks width and heights
  * Output is compared against reference values computed on the host.
* Expectations: Blocks of data are read correctly from buffer data types. Results match expected values
* Parameters:
  * Data types: uint, uint2, uint4, uint8, uchar, uchar2, uchar4, uchar8, uchar16, ushort, ushort2, ushort4, ushort8, ushort16
  * Dimensions: 2D
  * Object Types: images.
* Test case
  * Only read operation in kernel from image object.

### `sub_group_media_block_write`
* Status: Done
* Goal: Verify that `sub_group_media_block_write` function works as expected.
* Description:
  * Run OpenCL C kernel that calls `sub_group_media_block_write` in multiple work-groups, sub-groups, media blocks width and heights
  * Output is compared against reference values computed on the host.
* Expectations: Blocks of data are written correctly to image data types. Results match expected values
* Parameters:
  * Data types: uint, uint2, uint4, uint8, uchar, uchar2, uchar4, uchar8, uchar16, ushort, ushort2, ushort4, ushort8, ushort16
  * Dimensions: 2D
  * Object Types: images.
* Test case
  * Only write operation in kernel to image object.
