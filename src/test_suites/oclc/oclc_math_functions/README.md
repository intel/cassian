# oclc_math_functions

## Overview
Test suite for 
[math functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#math-functions) 
OpenCL C functionality.

## Tests

### Math functions
* Status: TODO
* Goal: Verify that math functions are the same on the device and the host with specified relative error(ULP).
* Description: Get the device return value from OpenCL C function and compare it with the value returned on the host taking ULP into account.
* Available modes: - TODO
  * `-cl-unsafe-math-optimizations` - passed to the compiler options
* Tested functions:
  * `gentype` - Single and double precision generic type function - TODO
    * `acos`, `acosh`, `acospi`, `asin`, `asinh`, `asinpi`, `atan`, `atan2`, `atanh`, `atanpi`, `atan2pi`, `cbrt`, `ceil`, `copysign`
  * `gentyped` - Double precision generic type function - TODO
  * `gentypef`- Single precision generic type function - TODO
  * `special` - Special functions that are not generic type - TODO
* Input values: 
  * `Basic` - Basic values for given function. TODO
  * `Special N` - Special values for given function. Covers [edge case behavior](https://www.khronos.org/registry/OpenCL/specs/3.0-unified/html/OpenCL_C.html#edge-case-behavior) of any function. `N` is the identifier of a spacial case, starting from 1.
  * `Denorms` - Denorm value for given function. TODO.
  * `Basic - random` - Part of Basic. Random values from function domain. (TODO: Each work item has different value to compute)

### Math macros
* Status: DONE
* Goal: Verify that math macros are the same on the device and the host.
* Description: Get the device value of a constant from executed kernel and compare it to the value on the host.
* Tested macros:
  * float type:
    * `FLT_DIG`, `FLT_MANT_DIG`, `FLT_MAX_10_EXP`, `FLT_MAX_EXP`, `FLT_MIN_10_EXP`, `FLT_MIN_EXP`, `FLT_RADIX`, `FLT_MAX`, `FLT_MIN`, `FLT_EPSILON`
  * double type:
    * `DBL_DIG`, `DBL_MANT_DIG`, `DBL_MAX_10_EXP`, `DBL_MAX_EXP`, `DBL_MIN_10_EXP`, `DBL_MIN_EXP`, `DBL_MAX`, `DBL_MIN`, `DBL_EPSILSON`

### Math constants
* Status: DONE
* Goal: Verify that math constants are the same on the device and the host.
* Description: Get the device value of a constant from executed kernel and compare it to the value on the host.
* Tested constants:
  * float type:
    * `M_E_F`, `M_LOG2E_F`, `M_LOG10E_F`, `M_LN2_F`, `M_LN10_F`, `M_PI_F`, `M_PI_2_F`, `M_PI_4_F`, `M_1_PI_F`, `M_2_PI_F`, `M_2_SQRTPI_F`, `M_SQRT2_F`, `M_SQRT1_2_F`, `MAXFLOAT`, `HUGE_VALF`, `INFINITY`, `NAN`
  * double type:
    * `M_E`, `M_LOG2E`, `M_LOG10E`, `M_LN2`, `M_LN10`, `M_PI`, `M_PI_2`, `M_PI_4`, `M_1_PI`, `M_2_PI`, `M_2_SQRTPI`, `M_SQRT2`, `M_SQRT1_2`, `HUGE_VAL`