# oclc_geometric_functions

## Overview
Test suite for 
[geometric functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#geometric-functions) 
OpenCL C functionality.

## Tests
### Geometric functions
* Status: TODO
* Goal: Verify that geometric function calculations are the same on the device and the host.
* Description: Get the device return value from OpenCL C function and compare it with the value returned on the host taking ULP into account.
* Tested functions:
  * `gentype` - Single and double precision generic type function
    * `cross`, `dot`, `distance`, `length`, `normalize`, `fast_distance`, `fast_length`, `fast_normalize`
* Input values:
  * `Random` - Random values from function domain, randomized across all of the work items.
  * `Special` - Special values for given function.
  * `Denorms`(TODO) - denormal values for given function.
