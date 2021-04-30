# oclc_common_functions

## Overview
Test suite for 
[common functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#common-functions) 
OpenCL C functionality.

## Tests
### common functions
* Status: TODO
* Goal: Verify that common function calculations are the same on the device and the host.
* Description: Get the device return value from OpenCL C function and compare it with the value returned on the host taking ULP into account.
* Tested functions:
  * `gentype` - Single and double precision generic type function.
    * `clamp`, `degrees`, `max`, `min`, `mix`, `radians`, `step`, `smoothstep`, `sign`
  * `gentypex` - Specific specialization of a single and double precision generic type function.
    * `clamp`, `max`, `min`, `mix`, `step`, `smoothstep`
* Input values:
  * `Random` - Random values from function domain, randomized across all of the work items.
  * `Special`(TODO) - Special values for given function.
  * `Denorms`(TODO) - Denormalized values.
