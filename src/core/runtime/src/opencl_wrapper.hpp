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

  cl_api_clGetPlatformIDs clGetPlatformIDs = nullptr;
  cl_api_clGetDeviceIDs clGetDeviceIDs = nullptr;
  cl_api_clGetDeviceInfo clGetDeviceInfo = nullptr;
  cl_api_clCreateContext clCreateContext = nullptr;
  cl_api_clCreateCommandQueueWithProperties clCreateCommandQueueWithProperties =
      nullptr;
  cl_api_clCreateBuffer clCreateBuffer = nullptr;
  cl_api_clCreateImage clCreateImage = nullptr;
  cl_api_clReleaseMemObject clReleaseMemObject = nullptr;
  cl_api_clCreateProgramWithSource clCreateProgramWithSource = nullptr;
  cl_api_clCreateProgramWithIL clCreateProgramWithIL = nullptr;
  cl_api_clReleaseProgram clReleaseProgram = nullptr;
  cl_api_clBuildProgram clBuildProgram = nullptr;
  cl_api_clGetProgramBuildInfo clGetProgramBuildInfo = nullptr;
  cl_api_clCreateKernel clCreateKernel = nullptr;
  cl_api_clReleaseKernel clReleaseKernel = nullptr;
  cl_api_clSetKernelArg clSetKernelArg = nullptr;
  cl_api_clFinish clFinish = nullptr;
  cl_api_clEnqueueReadBuffer clEnqueueReadBuffer = nullptr;
  cl_api_clEnqueueWriteBuffer clEnqueueWriteBuffer = nullptr;
  cl_api_clEnqueueNDRangeKernel clEnqueueNDRangeKernel = nullptr;
  cl_api_clEnqueueReadImage clEnqueueReadImage = nullptr;
  cl_api_clEnqueueWriteImage clEnqueueWriteImage = nullptr;
  cl_api_clReleaseContext clReleaseContext = nullptr;
  cl_api_clReleaseCommandQueue clReleaseCommandQueue = nullptr;
  cl_api_clCreateSampler clCreateSampler = nullptr;
  cl_api_clReleaseSampler clReleaseSampler = nullptr;
  cl_api_clCompileProgram clCompileProgram = nullptr;
  cl_api_clLinkProgram clLinkProgram = nullptr;
  cl_api_clGetProgramInfo clGetProgramInfo = nullptr;
  cl_api_clCreateSubDevices clCreateSubDevices = nullptr;

private:
  std::unique_ptr<Library> library_;
};

} // namespace cassian
#endif
