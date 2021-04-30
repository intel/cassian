# oclc_image_read_write_functions

## Overview
Test suite for [oclc image read and write functions](https://www.khronos.org/registry/OpenCL/specs/2.2/html/OpenCL_C.html#image-read-and-write-functions) OpenCL C functionality.

The goal of these tests is to verify the usage of all OpenCL C functions on images and samplers.

## Tests

### Program-scope sampler tests
* Status: WIP
* Goal: Verify that all OpenCL C read/write functions work properly with all types on different image types.
* Description: Run OpenCL C kernel with a defined sampler at program scope and do a kernel-copy image.
* Expectations: Image created from reference buffer is the same as the image created from the output buffer.

### Kernel argument sampler tests
* Status: WIP
* Goal: Verify that all OpenCL C read/write functions work properly with all types on different image types.
* Description: Run OpenCL C kernel with a defined sampler as kernel argument and do a kernel-copy image.
* Expectations: Image created from reference buffer is the same as the image created from the output buffer.

### Sampler-less tests
* Status: WIP
* Goal: Verify that all OpenCL C sampler-less read and write functions work properly with all types on different image types.
* Description: Run OpenCL C kernel without a sampler and do a kernel-copy image.
* Expectations: Image created from reference buffer is the same as the image created from the output buffer.

### Image read/write with specific qualifiers tests
* Status: TODO
* Goal: Verify that OpenCL C read and write functions work properly with image access qualifiers.
* Description: Run OpenCL C kernel and try to use read/write image functions on specific image access qualifiers.
* Expectations: Proper image access qualifier is properly used with image read/write function or throws.

### Sampler descriptor tests
* Status: TODO
* Goal: Verify that Sampler filter and addressing mode works properly.

### Tests for cl_khr_srgb_image_writes extension
* Status: TODO
* Goal: Verify that cl_khr_srgb_image_writes extension works properly.

### Tests for CL_DEVICE_MAX_CONSTANT_ARGS and CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE device queries
* Status: TODO
* Goal: Verify that the number of samplers in a kernel can be properly declared.
* Description: Samplers declared using the constant qualifier are not counted towards the maximum number of arguments
pointing to the constant address space or the maximum size of the constant address space allowed per device.
* Expectations: Kernel builts or throws.

### Tests for CL_DEVICE_MAX_SAMPLERS token in clGetDeviceInfo.
* Status: TODO
* Goal: Verify that the number of samplers in a kernel can be properly declared.
* Description: The maximum number of samplers that can be declared in a kernel can be queried using the CL_DEVICE_MAX_SAMPLERS token in clGetDeviceInfo.
* Expectations: Kernel builts or throws.

### Sampler syntax tests
* Status: TODO
* Goal: Verify that OpenCL C sampler declared using the constant qualifier are not counted towards the maximum number of arguments
pointing to the constant address space or the maximum size of the constant address space allowed per device.
* Description: Run OpenCL C kernel with a specifc syntax sampler, then query the device for
device max constant args and max constant buffer size.
* Expectations: Device query shows proper values.