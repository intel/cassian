/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_OPENCL_WRAPPER_HPP
#define CASSIAN_RUNTIME_OPENCL_WRAPPER_HPP

#include <CL/cl_icd.h>
#include <cassian/system/library.hpp>
#include <memory>

#define CL_DEVICE_SINGLE_FP_ATOMIC_CAPABILITIES_EXT 0x4231
#define CL_DEVICE_DOUBLE_FP_ATOMIC_CAPABILITIES_EXT 0x4232
#define CL_DEVICE_HALF_FP_ATOMIC_CAPABILITIES_EXT 0x4233

typedef cl_bitfield cl_device_fp_atomic_capabilities_ext;

#define CL_DEVICE_GLOBAL_FP_ATOMIC_LOAD_STORE_EXT (1 << 0)
#define CL_DEVICE_GLOBAL_FP_ATOMIC_ADD_EXT (1 << 1)
#define CL_DEVICE_GLOBAL_FP_ATOMIC_MIN_MAX_EXT (1 << 2)

#define CL_DEVICE_LOCAL_FP_ATOMIC_LOAD_STORE_EXT (1 << 16)
#define CL_DEVICE_LOCAL_FP_ATOMIC_ADD_EXT (1 << 17)
#define CL_DEVICE_LOCAL_FP_ATOMIC_MIN_MAX_EXT (1 << 18)

namespace cassian {
class OpenCLWrapper {
public:
  OpenCLWrapper();
  OpenCLWrapper(const OpenCLWrapper &) = delete;
  OpenCLWrapper(OpenCLWrapper &&) = delete;
  ~OpenCLWrapper() = default;
  OpenCLWrapper &operator=(const OpenCLWrapper &) = delete;
  OpenCLWrapper &operator=(OpenCLWrapper &&) = delete;

  clGetPlatformIDs_t *clGetPlatformIDs = nullptr;
  clGetDeviceIDs_t *clGetDeviceIDs = nullptr;
  clGetDeviceInfo_t *clGetDeviceInfo = nullptr;
  clCreateContext_t *clCreateContext = nullptr;
  clCreateCommandQueueWithProperties_t *clCreateCommandQueueWithProperties =
      nullptr;
  clCreateBuffer_t *clCreateBuffer = nullptr;
  clCreateImage_t *clCreateImage = nullptr;
  clReleaseMemObject_t *clReleaseMemObject = nullptr;
  clCreateProgramWithSource_t *clCreateProgramWithSource = nullptr;
  clCreateProgramWithIL_t *clCreateProgramWithIL = nullptr;
  clReleaseProgram_t *clReleaseProgram = nullptr;
  clBuildProgram_t *clBuildProgram = nullptr;
  clGetProgramBuildInfo_t *clGetProgramBuildInfo = nullptr;
  clCreateKernel_t *clCreateKernel = nullptr;
  clReleaseKernel_t *clReleaseKernel = nullptr;
  clSetKernelArg_t *clSetKernelArg = nullptr;
  clFinish_t *clFinish = nullptr;
  clEnqueueReadBuffer_t *clEnqueueReadBuffer = nullptr;
  clEnqueueWriteBuffer_t *clEnqueueWriteBuffer = nullptr;
  clEnqueueNDRangeKernel_t *clEnqueueNDRangeKernel = nullptr;
  clEnqueueReadImage_t *clEnqueueReadImage = nullptr;
  clEnqueueWriteImage_t *clEnqueueWriteImage = nullptr;
  clReleaseContext_t *clReleaseContext = nullptr;
  clReleaseCommandQueue_t *clReleaseCommandQueue = nullptr;
  clCreateSampler_t *clCreateSampler = nullptr;
  clReleaseSampler_t *clReleaseSampler = nullptr;
  clCompileProgram_t *clCompileProgram = nullptr;
  clLinkProgram_t *clLinkProgram = nullptr;
  clGetProgramInfo_t *clGetProgramInfo = nullptr;
  clCreateSubDevices_t *clCreateSubDevices = nullptr;

private:
  std::unique_ptr<Library> library_;
};

} // namespace cassian
#endif
