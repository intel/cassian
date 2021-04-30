/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_OPENCL_WRAPPER_HPP
#define CASSIAN_RUNTIME_OPENCL_WRAPPER_HPP

#include <CL/cl_icd.h>
#include <cassian/system/library.hpp>
#include <memory>

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

private:
  std::unique_ptr<Library> library_;
};

} // namespace cassian
#endif
