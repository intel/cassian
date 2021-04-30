# oclc_integer_functions

## Overview
Test suite for 
[integer functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#integer-functions) 
OpenCL C functionality.

## Tests

### Integer functions
* Status: TODO
* Goal: Verify that device output value from integer functions are the same as values calculated on the host.
* Description: Get the device return value from OpenCL C function and compare it with the value returned on the host.
* Tested functions:
  * `abs`, `abs_diff`, `add_sat`, `hadd`, `rhadd`, `clamp`, `clz`, `ctz`(TODO), `mad_hi`(TODO), `mad_sat`(TODO), `max`(TODO), `min`(TODO), `mul_hi`(TODO), `rotate`(TODO), `sub_sat`(TODO), `upsample`, `popcount`(TODO), `mad24`(TODO), `mul24`(TODO),
* Input values(TODO: implement more granular logic):
  * Simple input from domain

### Integer macros
* Status: DONE
* Goal: Verify that integer macros are the same on the device and the host.
* Description: Get the device value of a constant from executed kernel and compare it to the value on the host.
* Tested macros:
  * `CHAR_BIT`, `CHAR_MAX`, `CHAR_MIN`, `INT_MAX`, `INT_MIN`, `LONG_MAX`, `LONG_MIN`, `SCHAR_MAX`, `SCHAR_MIN`, `SHRT_MAX`, `SHRT_MIN`, `UCHAR_MAX`, `USHRT_MAX`, `UINT_MAX`, `ULONG_MAX`