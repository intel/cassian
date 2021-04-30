# oclc_access_qualifiers

## Overview
Test suite for [access qualifiers](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#access-qualifiers) OpenCL C functionality.

The goal of those tests is to verify use of access qualifiers: read_only, write_only, read_write on images as kernel and function arguments.

## Tests

### Image as read_only image2d_t kernel argument
* Status: DONE
* Goal: Test read_only qualifier with image2d_t as kernel argument.
* Description: Test creates a basic image with a qualifier flag set to read_only. Then image is set as a kernel argument to other qualifiers and evaluates if it is building and executing.
* Expectations: Kernel is sucessfully compiled and ran or throws an error.

### Image as write_only image2d_t kernel argument
* Status: DONE
* Goal: Test write_only qualifier with image2d_t as kernel argument.
* Description: Test creates a basic image with a qualifier flag set to write_only. Then image is set as a kernel argument to other qualifiers and evaluates if it is building and executing.
* Expectations: Kernel is sucessfully compiled and ran or throws an error.

### Image as read_write image2d_t kernel argument
* Status: DONE
* Goal: Test read_write qualifier with image2d_t as kernel argument.
* Description: Test creates a basic image with a qualifier flag set to read_write. Then image is set as a kernel argument to other qualifiers and evaluates if it is building and executing.
* Expectations: Kernel is sucessfully compiled and ran or throws an error.

### Image as read_only image1d_t/image3d_t kernel argument
* Status: TODO
* Goal: Test read_only qualifier with image1d_t/image3d_t as kernel argument.
* Description: Test creates a basic image with a qualifier flag set to read_only. Then image is set as a kernel argument to other qualifiers and evaluates if it is building and executing.
* Expectations: Kernel is sucessfully compiled and ran or throws an error.

### Image as write_only image1d_t/image3d_t kernel argument
* Status: TODO
* Goal: Test write_only qualifier with image1d_t/image3d_t as kernel argument.
* Description: Test creates a basic image with a qualifier flag set to write_only. Then image is set as a kernel argument to other qualifiers and evaluates if it is building and executing.
* Expectations: Kernel is sucessfully compiled and ran or throws an error.

### Image as read_write image1d_t/image3d_t kernel argument
* Status: TODO
* Goal: Test read_write qualifier with image1d_t/image3d_t as kernel argument.
* Description: Test creates a basic image with a qualifier flag set to read_write. Then image is set as a kernel argument to other qualifiers and evaluates if it is building and executing.
* Expectations: Kernel is sucessfully compiled and ran or throws an error.

### Function arguments access qualifiers tests
* Status: TODO
