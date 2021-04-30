# oclc_conversions

## Overview
Test suite for [data type conversions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#conversions-and-type-casting) OpenCL C functionality.

The goal of these tests is to verify conversions between all supported data types.

Tests are done using specific value. TODO: multiple patterns.

## Tests

### Implicit conversion

* Status: DONE
* Goal: Verify that implicit conversion produces correct value.
* Description: Run OpenCL C kernel that assigns a value, converts it implicitly and then returns that value in output.
* Expectations: Returned value is correct.

### Explicit cast

* Status: DONE
* Goal: Verify that explicit cast produces correct values.
* Description: Run OpenCL C kernel that assigns a value, converts it explicitly to scalar and vector types by casting and then returns that value or values in output. For vectors the converted value should be copied to all elements. Values are then returned in output.
* Expectations: Returned values are correct.

### Explicit conversion

* Status: DONE
* Goal: Verify that explicit conversion produces correct values.
* Description: Run OpenCL C kernel that assigns a value, converts it explicitly to scalar and vector types using convert_* function and then returns that value or values in output. For vectors all elements should be converted. Values are then returned in output.
* Expectations: Returned values are correct.

#### Rounding and saturation modes

* Status: TODO
* Goal: Verify that explicit conversion with rounding and/or saturation produces correct values.
* Description: Run OpenCL C kernel that assigns a value, converts it explicitly to scalar and vector types using convert_* function and then returns that value or values in output. For vectors all elements should be converted. Values are then returned in output.
* Expectations: Returned values are correct.

### Reinterpreting

#### Using unions

* Status: TODO
* Goal: Verify that reinterpretation using unions produces correct values.
* Description: Run OpenCL C kernel that assigns a value, reinterprets it using union and then returns that value or values in output. Values are then returned in output.
* Expectations: Returned values are correct.

#### Using as_type

* Status: TODO
* Goal: Verify that reinterpretation using as_type produces correct values.
* Description: Run OpenCL C kernel that assigns a value, reinterprets it using as_type and then returns that value or values in output. Values are then returned in output.
* Expectations: Returned values are correct.

### Pointer casting

* Status: TODO
* Goal: Verify that reinterpretation using pointer casting produces correct values.
* Description: Run OpenCL C kernel that assigns a value, reinterprets it using pointer casting and then returns that value or values in output. Values are then returned in output.
* Expectations: Returned values are correct.

### Arithmetic conversions

* Status: TODO
* Goal: Verify that conversion with arithmetic operations produces correct values.
* Description: Run OpenCL C kernel that assigns a value, performs arithmetic operation that results in conversion and then returns that value or values in output. Values are then returned in output.
* Expectations: Returned values are correct.
