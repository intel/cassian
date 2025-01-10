/*
 * Copyright (C) 2021-2023 Intel Corporation
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

  clGetPlatformIDs = reinterpret_cast<clGetPlatformIDs_t *>(
      library_->get_function("clGetPlatformIDs"));
  clGetDeviceIDs = reinterpret_cast<clGetDeviceIDs_t *>(
      library_->get_function("clGetDeviceIDs"));
  clGetDeviceInfo = reinterpret_cast<clGetDeviceInfo_t *>(
      library_->get_function("clGetDeviceInfo"));
  clCreateContext = reinterpret_cast<clCreateContext_t *>(
      library_->get_function("clCreateContext"));
  clCreateCommandQueueWithProperties =
      reinterpret_cast<clCreateCommandQueueWithProperties_t *>(
          library_->get_function("clCreateCommandQueueWithProperties"));
  clCreateBuffer = reinterpret_cast<clCreateBuffer_t *>(
      library_->get_function("clCreateBuffer"));
  clCreateImage = reinterpret_cast<clCreateImage_t *>(
      library_->get_function("clCreateImage"));
  clReleaseMemObject = reinterpret_cast<clReleaseMemObject_t *>(
      library_->get_function("clReleaseMemObject"));
  clCreateProgramWithSource = reinterpret_cast<clCreateProgramWithSource_t *>(
      library_->get_function("clCreateProgramWithSource"));
  clCreateProgramWithIL = reinterpret_cast<clCreateProgramWithIL_t *>(
      library_->get_function("clCreateProgramWithIL"));
  clReleaseProgram = reinterpret_cast<clReleaseProgram_t *>(
      library_->get_function("clReleaseProgram"));
  clBuildProgram = reinterpret_cast<clBuildProgram_t *>(
      library_->get_function("clBuildProgram"));
  clGetProgramBuildInfo = reinterpret_cast<clGetProgramBuildInfo_t *>(
      library_->get_function("clGetProgramBuildInfo"));
  clCreateKernel = reinterpret_cast<clCreateKernel_t *>(
      library_->get_function("clCreateKernel"));
  clReleaseKernel = reinterpret_cast<clReleaseKernel_t *>(
      library_->get_function("clReleaseKernel"));
  clSetKernelArg = reinterpret_cast<clSetKernelArg_t *>(
      library_->get_function("clSetKernelArg"));
  clFinish = reinterpret_cast<clFinish_t *>(library_->get_function("clFinish"));
  clEnqueueReadBuffer = reinterpret_cast<clEnqueueReadBuffer_t *>(
      library_->get_function("clEnqueueReadBuffer"));
  clEnqueueReadImage = reinterpret_cast<clEnqueueReadImage_t *>(
      library_->get_function("clEnqueueReadImage"));
  clEnqueueWriteImage = reinterpret_cast<clEnqueueWriteImage_t *>(
      library_->get_function("clEnqueueWriteImage"));
  clEnqueueWriteBuffer = reinterpret_cast<clEnqueueWriteBuffer_t *>(
      library_->get_function("clEnqueueWriteBuffer"));
  clEnqueueNDRangeKernel = reinterpret_cast<clEnqueueNDRangeKernel_t *>(
      library_->get_function("clEnqueueNDRangeKernel"));
  clReleaseContext = reinterpret_cast<clReleaseContext_t *>(
      library_->get_function("clReleaseContext"));
  clReleaseCommandQueue = reinterpret_cast<clReleaseCommandQueue_t *>(
      library_->get_function("clReleaseCommandQueue"));
  clCreateSampler = reinterpret_cast<clCreateSampler_t *>(
      library_->get_function("clCreateSampler"));
  clReleaseSampler = reinterpret_cast<clReleaseSampler_t *>(
      library_->get_function("clReleaseSampler"));
  clCompileProgram = reinterpret_cast<clCompileProgram_t *>(
      library_->get_function("clCompileProgram"));
  clLinkProgram = reinterpret_cast<clLinkProgram_t *>(
      library_->get_function("clLinkProgram"));
  clGetProgramInfo = reinterpret_cast<clGetProgramInfo_t *>(
      library_->get_function("clGetProgramInfo"));
  clCreateSubDevices = reinterpret_cast<clCreateSubDevices_t *>(
      library_->get_function("clCreateSubDevices"));
}
} // namespace cassian
