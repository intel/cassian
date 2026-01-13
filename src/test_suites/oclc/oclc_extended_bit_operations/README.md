# oclc_extended_bit_operations
## Overview

Test for bit instructions.

Tests extension [cl_khr_extended_bit_ops](https://registry.khronos.org/OpenCL/specs/3.0-unified/html/OpenCL_C.html#extended-bit-operations).

## Tests

### bitfield_insert
* Status: Done
* Goal: Verify bitfield_insert function from `cl_khr_extended_bit_ops` extension.
* Description: Test performs bit insertion using function bitfield_insert.
* Expectations: Output matches calculated reference values.

### bitfield_extract_signed
* Status: Done
* Goal: Verify bitfield_extract_signed function from `cl_khr_extended_bit_ops` extension.
* Description: Test performs bit extraction into signed type using function bitfield_extract_signed.
* Expectations: Output matches calculated reference values.

### bitfield_extract_unsigned
* Status: Done
* Goal: Verify bitfield_extract_unsigned function from `cl_khr_extended_bit_ops` extension.
* Description: Test performs bit extraction into unsigned type using function bitfield_extract_unsigned.
* Expectations: Output matches calculated reference values.

### bit_reverse
* Status: Done
* Goal: Verify bit_reverse function from `cl_khr_extended_bit_ops` extension.
* Description: Test performs bit reverse using function bit_reverse.
* Expectations: Output matches calculated reference values.
