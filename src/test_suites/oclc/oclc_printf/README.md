# oclc_printf

## Overview
Test suite for [printf](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#printf) OpenCL C functionality.

The goal of these tests is to verify output of printf function.

## Tests

### Text

* Status: DONE
* Goal: Verify that printf function prints static text.
* Description: Run OpenCL C kernel with printf call.
* Expectations: Text passed to printf function should be printed to stdout.

### Arguments

* Status: DONE
* Goal: Verify that printf function prints arguments in specified format.
* Description: Run OpenCL C kernel with printf call.
* Expectations: Text with arguments passed to printf function should be printed to stdout with correct formatting.
