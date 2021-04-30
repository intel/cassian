# oclc_attribute_qualifiers

## Overview
Test suite for [attribute qualifiers](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#attribute-qualifiers) OpenCL C functionality.

The goal of these tests is to verify attribute qualifiers.

## Tests

### Attributes of types

#### aligned

* Status: DONE
* Goal: Verify that type alignment attribute parameters are properly working.
* Description: Run OpenCL C kernel with given alignment parameters.
* Expectations: Variables of types with defined alignment should compile.

#### packed

* Status: DONE
* Goal: Verify that type attribute `packed` is properly working.
* Description: Run OpenCL C kernel with given alignment parameters.
* Expectations: Variables of packed types should have smallest possible size.

### Attributes of variables

#### aligned

* Status: DONE
* Goal: Verify that type alignment attribute parameters are properly working.
* Description: Run OpenCL C kernel with given alignment parameters.
* Expectations: Variables with defined alignment should compile.

#### packed

* Status: DONE
* Goal: Verify that variable attribute `packed` is properly working.
* Description: Run OpenCL C kernel with given alignment parameters.
* Expectations: Packed variables should have smallest possible size.

#### endian

* Status: TODO
* Goal: Verify that variable attribute endian parameters are properly working.
* Description: Run OpenCL C kernel with given endianness parameters.
* Expectations: Operations on variables with endianness defined should follow that endianness.

### Unrolling loops

* Status: DONE
* Goal: Verify that loop unroll attribute parameters are properly working.
* Description: Run OpenCL C kernel with given optimization parameters.
* Expectations: Kernel with loop unroll parameters compile correctly and unrolled loop gives correct results.
