# oclc_work_item_functions

## Overview
Test suite for 
[work-item functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#work-item-functions) 
OpenCL C functionality.

## Tests

### `get_work_dim`
* Status: DONE
* Goal: Verify that `get_work_dim` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_work_dim`.
  * Output is compared against reference values computed on the host.
* Expectations: Returned value is the same as enqueued NDRange dimension.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  
#### `get_work_dim - wrappers`
* Status: DONE
* Goal: Verify that outside the kernel `get_work_dim` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_work_dim` via the wrapper.
  * Output is compared against reference values computed on the host.
* Expectations: Returned value is the same as enqueued NDRange dimension.
* Parameters:
  * Dimensions: 1D.

### `get_global_size`
* Status: TODO
* Goal: Verify that `get_global_size` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_global_size`.
  * Output is compared against reference values computed on the host.
* Expectations: Returned value is the same as global work size passed to enqueue function.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_global_id`
* Status: DONE
* Goal: Verify that `get_global_id` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_global_id`.
  * Output is compared against reference values computed on the host.
* Expectations: Returned values contain all ids within enqueued global work size.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  
#### `get_global_id - wrappers`
* Status: DONE
* Goal: Verify that outside the kernel `get_global_id` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_global_id` via the wrapper.
  * Output is compared against reference values computed on the host.
* Expectations: Returned values contain all ids within enqueued global work size.
* Parameters:
  * Dimensions: 3D.

### `get_local_size`
* Status: TODO
* Goal: Verify that `get_local_size` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_local_size` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Returned value is the same as local work size passed to enqueue function.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_enqueued_local_size`
* Status: TODO
* Goal: Verify that `get_enqueued_local_size` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_enqueued_local_size` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Returned value is the same as enqueued local work size.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  * Work-group size: uniform, non-uniform.

### `get_local_id`
* Status: DONE
* Goal: Verify that `get_local_id` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_local_id` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Returned values contain all ids within enqueued local work sizes.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
  
#### `get_local_id - wrappers`
* Status: DONE
* Goal: Verify that outside the kernel `get_local_id` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_local_id` in multiple work-groups via the wrapper.
  * Output is compared against reference values computed on the host.
* Expectations: Returned values contain all ids within enqueued local work sizes.
* Parameters:
  * Dimensions: 3D.

### `get_num_groups`
* Status: TODO
* Goal: Verify that `get_num_groups` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_num_groups`.
  * Output is compared against reference values computed on the host.
* Expectations: Returned values is the same as number of work-groups defined as gws/lws.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_group_id`
* Status: TODO
* Goal: Verify that `get_group_id` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_group_id` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Returned values contain all ids within enqueued work-groups.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_global_offset`
* Status: TODO
* Goal: Verify that `get_global_offset` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_global_offset`.
  * Output is compared against reference values computed on the host.
* Expectations: Returned value is the same as global work offset passed to enqueue function.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_global_linear_id`
* Status: TODO
* Goal: Verify that `get_global_linear_id` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_global_linear_id`.
  * Output is compared against reference values computed on the host.
* Expectations: Returned values map into linear id space within global work size.
* Parameters:
  * Dimensions: 1D, 2D, 3D.

### `get_local_linear_id`
* Status: TODO
* Goal: Verify that `get_local_linear_id` function works as expected.
* Description: 
  * Run OpenCL C kernel that calls `get_local_linear_id` in multiple work-groups.
  * Output is compared against reference values computed on the host.
* Expectations: Returned values map into linear id space within local work size.
* Parameters:
  * Dimensions: 1D, 2D, 3D.
