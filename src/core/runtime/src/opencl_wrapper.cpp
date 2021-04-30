/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <CL/cl_icd.h>
#include <cassian/system/factory.hpp>
#include <cassian/system/library.hpp>
#include <opencl_wrapper.hpp>

#if defined(_WIN32)
constexpr char ocl_loader[] = "OpenCL.dll";
#elif defined(__linux__)
constexpr char ocl_loader[] = "libOpenCL.so.1";
#endif

namespace cassian {
OpenCLWrapper::OpenCLWrapper() {
  library_ = load_library(ocl_loader);

  clGetPlatformIDs = reinterpret_cast<cl_api_clGetPlatformIDs>(
      library_->get_function("clGetPlatformIDs"));
  clGetDeviceIDs = reinterpret_cast<cl_api_clGetDeviceIDs>(
      library_->get_function("clGetDeviceIDs"));
  clGetDeviceInfo = reinterpret_cast<cl_api_clGetDeviceInfo>(
      library_->get_function("clGetDeviceInfo"));
  clCreateContext = reinterpret_cast<cl_api_clCreateContext>(
      library_->get_function("clCreateContext"));
  clCreateCommandQueueWithProperties =
      reinterpret_cast<cl_api_clCreateCommandQueueWithProperties>(
          library_->get_function("clCreateCommandQueueWithProperties"));
  clCreateBuffer = reinterpret_cast<cl_api_clCreateBuffer>(
      library_->get_function("clCreateBuffer"));
  clCreateImage = reinterpret_cast<cl_api_clCreateImage>(
      library_->get_function("clCreateImage"));
  clReleaseMemObject = reinterpret_cast<cl_api_clReleaseMemObject>(
      library_->get_function("clReleaseMemObject"));
  clCreateProgramWithSource =
      reinterpret_cast<cl_api_clCreateProgramWithSource>(
          library_->get_function("clCreateProgramWithSource"));
  clCreateProgramWithIL = reinterpret_cast<cl_api_clCreateProgramWithIL>(
      library_->get_function("clCreateProgramWithIL"));
  clReleaseProgram = reinterpret_cast<cl_api_clReleaseProgram>(
      library_->get_function("clReleaseProgram"));
  clBuildProgram = reinterpret_cast<cl_api_clBuildProgram>(
      library_->get_function("clBuildProgram"));
  clGetProgramBuildInfo = reinterpret_cast<cl_api_clGetProgramBuildInfo>(
      library_->get_function("clGetProgramBuildInfo"));
  clCreateKernel = reinterpret_cast<cl_api_clCreateKernel>(
      library_->get_function("clCreateKernel"));
  clReleaseKernel = reinterpret_cast<cl_api_clReleaseKernel>(
      library_->get_function("clReleaseKernel"));
  clSetKernelArg = reinterpret_cast<cl_api_clSetKernelArg>(
      library_->get_function("clSetKernelArg"));
  clFinish =
      reinterpret_cast<cl_api_clFinish>(library_->get_function("clFinish"));
  clEnqueueReadBuffer = reinterpret_cast<cl_api_clEnqueueReadBuffer>(
      library_->get_function("clEnqueueReadBuffer"));
  clEnqueueReadImage = reinterpret_cast<cl_api_clEnqueueReadImage>(
      library_->get_function("clEnqueueReadImage"));
  clEnqueueWriteImage = reinterpret_cast<cl_api_clEnqueueWriteImage>(
      library_->get_function("clEnqueueWriteImage"));
  clEnqueueWriteBuffer = reinterpret_cast<cl_api_clEnqueueWriteBuffer>(
      library_->get_function("clEnqueueWriteBuffer"));
  clEnqueueNDRangeKernel = reinterpret_cast<cl_api_clEnqueueNDRangeKernel>(
      library_->get_function("clEnqueueNDRangeKernel"));
  clReleaseContext = reinterpret_cast<cl_api_clReleaseContext>(
      library_->get_function("clReleaseContext"));
  clReleaseCommandQueue = reinterpret_cast<cl_api_clReleaseCommandQueue>(
      library_->get_function("clReleaseCommandQueue"));
  clCreateSampler = reinterpret_cast<cl_api_clCreateSampler>(
      library_->get_function("clCreateSampler"));
  clReleaseSampler = reinterpret_cast<cl_api_clReleaseSampler>(
      library_->get_function("clReleaseSampler"));
  clCompileProgram = reinterpret_cast<cl_api_clCompileProgram>(
      library_->get_function("clCompileProgram"));
  clLinkProgram = reinterpret_cast<cl_api_clLinkProgram>(
      library_->get_function("clLinkProgram"));
}
} // namespace cassian
