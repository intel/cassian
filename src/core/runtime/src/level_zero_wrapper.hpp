/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_LEVEL_ZERO_WRAPPER_HPP
#define CASSIAN_RUNTIME_LEVEL_ZERO_WRAPPER_HPP

#include <cassian/system/library.hpp>
#include <memory>
#include <ze_ddi.h>

namespace cassian {
class LevelZeroWrapper {
public:
  LevelZeroWrapper();
  LevelZeroWrapper(const LevelZeroWrapper &) = delete;
  LevelZeroWrapper(LevelZeroWrapper &&) = delete;
  ~LevelZeroWrapper() = default;
  LevelZeroWrapper &operator=(const LevelZeroWrapper &) = delete;
  LevelZeroWrapper &operator=(LevelZeroWrapper &&) = delete;

  ze_pfnInit_t zeInit = nullptr;
  ze_pfnDriverGet_t zeDriverGet = nullptr;
  ze_pfnInitDrivers_t zeInitDrivers = nullptr;
  ze_pfnDeviceGet_t zeDeviceGet = nullptr;
  ze_pfnDeviceGetProperties_t zeDeviceGetProperties = nullptr;
  ze_pfnDeviceGetModuleProperties_t zeDeviceGetModuleProperties = nullptr;
  ze_pfnDeviceGetComputeProperties_t zeDeviceGetComputeProperties = nullptr;
  ze_pfnContextCreate_t zeContextCreate = nullptr;
  ze_pfnContextDestroy_t zeContextDestroy = nullptr;
  ze_pfnCommandQueueCreate_t zeCommandQueueCreate = nullptr;
  ze_pfnCommandQueueDestroy_t zeCommandQueueDestroy = nullptr;
  ze_pfnCommandQueueExecuteCommandLists_t zeCommandQueueExecuteCommandLists =
      nullptr;
  ze_pfnCommandQueueSynchronize_t zeCommandQueueSynchronize = nullptr;
  ze_pfnCommandListCreate_t zeCommandListCreate = nullptr;
  ze_pfnCommandListReset_t zeCommandListReset = nullptr;
  ze_pfnCommandListDestroy_t zeCommandListDestroy = nullptr;
  ze_pfnCommandListClose_t zeCommandListClose = nullptr;
  ze_pfnModuleCreate_t zeModuleCreate = nullptr;
  ze_pfnModuleDestroy_t zeModuleDestroy = nullptr;
  ze_pfnModuleDynamicLink_t zeModuleDynamicLink = nullptr;
  ze_pfnKernelCreate_t zeKernelCreate = nullptr;
  ze_pfnKernelDestroy_t zeKernelDestroy = nullptr;
  ze_pfnKernelSetGroupSize_t zeKernelSetGroupSize = nullptr;
  ze_pfnKernelSuggestGroupSize_t zeKernelSuggestGroupSize = nullptr;
  ze_pfnKernelSetArgumentValue_t zeKernelSetArgumentValue = nullptr;
  ze_pfnCommandListAppendLaunchKernel_t zeCommandListAppendLaunchKernel =
      nullptr;
  ze_pfnMemAllocShared_t zeMemAllocShared = nullptr;
  ze_pfnMemAllocDevice_t zeMemAllocDevice = nullptr;
  ze_pfnMemFree_t zeMemFree = nullptr;
  ze_pfnCommandListAppendMemoryCopy_t zeCommandListAppendMemoryCopy = nullptr;
  ze_pfnCommandListAppendImageCopyToMemory_t
      zeCommandListAppendImageCopyToMemory = nullptr;
  ze_pfnCommandListAppendImageCopyFromMemory_t
      zeCommandListAppendImageCopyFromMemory = nullptr;
  ze_pfnImageCreate_t zeImageCreate = nullptr;
  ze_pfnImageDestroy_t zeImageDestroy = nullptr;
  ze_pfnDeviceGetImageProperties_t zeDeviceGetImageProperties = nullptr;
  ze_pfnSamplerCreate_t zeSamplerCreate = nullptr;
  ze_pfnSamplerDestroy_t zeSamplerDestroy = nullptr;
  ze_pfnModuleBuildLogGetString_t zeModuleBuildLogGetString = nullptr;
  ze_pfnModuleBuildLogDestroy_t zeModuleBuildLogDestroy = nullptr;
  ze_pfnImageViewCreateExp_t zeImageViewCreateExp = nullptr;
  ze_pfnModuleGetNativeBinary_t zeModuleGetNativeBinary = nullptr;
  ze_pfnDeviceGetSubDevices_t zeDeviceGetSubDevices = nullptr;

private:
  std::unique_ptr<Library> library_;
};

} // namespace cassian
#endif
