# oclc_address_space_qualifiers

## Overview
Test suite for [address space qualifiers](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#address-space-qualifiers) OpenCL C functionality.

The goal of those tests is to verify use of address space qualifiers: global, local, constant, private on kernel arguments and variables. This way test verifies API functionality and use of various memory regions.

## Tests

### Kernel arguments address space qualifiers tests

#### Local argument buffers read and write with local memory sharing
* Status: WIP
* Goal: Verify read write operations on local memory buffer passed as an argument and verify local memory sharing between work items in local workgroup. 
* Description: Test copies input buffer to output buffer through local memory buffer. Each work item reads local memory chunk written by different work item to verify memory sharing.
* Expectations: Input and output buffer are the same.

#### Global buffer arguments read and write
* Status: WIP
* Goal: Verify read and write operations on global buffer.
* Description: Test evaluates square of passed values on inout buffer.
* Expectations: Values inside inout buffer are replaced by square of them.

#### Constant buffer arguments read
* Status: WIP
* Goal: Verify read operations on constant buffer.
* Description: Test sums all input values in the global work group per work item.
* Expectations: Each value in output buffer is sum of all input values.

### Kernel variables address spaces test
* Status: TODO
