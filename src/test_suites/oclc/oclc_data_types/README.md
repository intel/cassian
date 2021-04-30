# oclc_data_types

## Overview
Test suite for [data types](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#supported-data-types) OpenCL C functionality.

The goal of these tests is to verify definitions of all supported data types.

Tests are done using linear sequence for vector tests and a specific value for scalar tests.

## Tests

### Declaration

* Status: DONE
* Goal: Verify that declaring scalar value produces correct value.
* Description: Run OpenCL C kernel that assigns a value and then returns that value in output.
* Expectations: Returned value is correct.

### Vector literals

* Status: DONE
* Goal: Verify that using vector constructors produces correct values.
* Description: Run OpenCL C kernel that assigns values to a vector using one of specified constructors. Different combinations are tested: float2 values to construct float4, etc.; only one level deep. Values are then return in output.
* Expectations: Returned values are correct.

### Vector components

#### Reading

* Status: DONE
* Goal: Verify that using vector components to access elements of a vector produces correct values.
* Description: Run OpenCL C kernel that assigns values to a vector using basic constructor. Values are then accessed using specified component mix and variant and returned in output.
* Expectations: Returned values are correct.

#### Writing

* Status: TODO
* Goal: Verify that using vector components to write elements of a vector overwrites correct values.
* Description: Run OpenCL C kernel that assigns values to a vector using basic constructor. Values are then overwritten using specified component mix and variant and vector is returned in output.
* Expectations: Returned values are correct.
