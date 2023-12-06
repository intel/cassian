# oclc_math_functions

## Overview
Test suite for [math functions](https://registry.khronos.org/OpenCL/specs/3.0-unified/html/OpenCL_C.html#math-functions) OpenCL C functionality.

## Tests

### Math functions
* Status: TODO
* Goal: Verify that math functions are the same on the device and the host with specified relative error(ULP).
* Description: Get the device return value from OpenCL C function and compare it with the value returned on the host taking ULP into account.
  ULP comparison is not functional for native_* functions because their precision is implementation-defined.
* Available modes: - TODO
  * `-cl-unsafe-math-optimizations` - passed to the compiler options
* Input values: 
  * `Random` - Random value from the function domain. This value is used to fill the whole work size with std::nextafter of the previous work item value.
  * `Edge Case` - Edge values for given function. Covers [edge case behavior](https://www.khronos.org/registry/OpenCL/specs/3.0-unified/html/OpenCL_C.html#edge-case-behavior) of every function.
  * `Denorms` TODO - Denorm value for given function.
* Tests:
  * `math_functions_gentype` - Single and double precision generic type functions.
    * `acos`
    * `acosh`
    * `acospi`
    * `asin`
    * `asinh`
    * `asinpi`
    * `atan`
    * `atan2`
    * `atanh`
    * `atanpi`
    * `atan2pi`
    * `cbrt`
    * `ceil`
    * `copysign`
    * `cos`
    * `cosh`
    * `cospi`
    * `erfc`
    * `erf`
    * `exp`
    * `exp2`
    * `exp10`
    * `expm1`
    * `fabs`
    * `fdim`
    * `floor`
    * `fma`
    * `fmax`
    * `fmin`
    * `fmod`
    * `hypot`
    * `ilogb` 
    * `ldexp`
    * `lgamma`
    * `log`
    * `log2`
    * `log10`
    * `log1p`
    * `logb`
    * `mad`
    * `maxmag`
    * `minmag`
    * `nan`
    * `nextafter`
    * `pow`
    * `pown`
    * `powr`
    * `remainder`
    * `rint`
    * `rootn`
    * `round`
    * `rsqrt`
    * `sinh`
    * `sinpi`
    * `sqrt`
    * `tan`
    * `tanh`
    * `tanpi`
    * `tgamma`
    * `trunc`
    * `half_cos`
    * `half_divide`
    * `half_exp`
    * `half_exp2`
    * `half_exp10`
    * `half_log`
    * `half_log2`
    * `half_log10`
    * `half_powr`
    * `half_recip`
    * `half_rsqrt`
    * `half_sin`
    * `half_sqrt`
    * `half_tan`
    * `native_cos`
    * `native_divide`
    * `native_exp`
    * `native_exp2`
    * `native_exp10`
    * `native_log`
    * `native_log2`
    * `native_log10`
    * `native_powr`
    * `native_recip`
    * `native_rsqrt`
    * `native_sin`
    * `native_sqrt`
    * `native_tan`
  * `math_functions_specializations` - Specialization of given functions.
    * `fmax` - gentype fmax(gentype x, float/double y)
    * `fmin` - gentype fmin(gentype x, float/double y)
    * `ldexp` - gentype ldexp(gentype x, int k)
  * `math_function_store_functions` - Store functions.
    * `fract`
    * `frexp`
    * `lgamma_r`
    * `modf`
    * `remquo`
    * `sincos`

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