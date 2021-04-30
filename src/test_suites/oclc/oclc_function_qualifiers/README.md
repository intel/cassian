# oclc_function_qualifiers

## Overview
Test suite for [function qualifiers](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#function-qualifiers) OpenCL C functionality.

The goal of these tests is to verify function qualifiers, i.e all of the attribute qualifiers.

## Tests

### Attribute reqd_work_group_size

* Status: DONE
* Goal: Verify that kernel attribute parameters are properly working with given local work size.
* Description: Run OpenCL C kernel with given optimization parameters.
* Expectations: Queried work group size is equal to the host expected values across multiple work groups or given invalid work size kernel should fail on execution.

### Attribute work_group_size_hint

* Status: TODO
* Goal: Verify that kernel attribute parameters are properly working with given local work size.
* Description: Run OpenCL C kernel with given optimization parameters.
* Expectations: Kernel with attribute is built and executed successfully.

### Attribute vec_type_hint

* Status: TODO
* Goal: Verify that kernel attribute is properly working.
* Description: Run OpenCL C kernel with given optimization parameters.
* Expectations: Kernel with attribute is built and executed successfully.

### Multiple attributes

* Status: TODO
* Goal: Verify that above's attributes can work together with a kernel function.
* Description: Run OpenCL C kernel with multiple attributes.
* Expectations: Multiple attributes work together combined.

### Multiple kernels in a single program

* Status: DONE
* Goal: Verify that one kernel can call another one.
* Description: Run OpenCL C kernel that calls another kernel.
* Expectations: Buffer is properly copied inside kernel.
