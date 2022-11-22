/*
 * Copyright (C) 2021-2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/system/factory.hpp>
#include <cassian/system/library.hpp>
#include <level_zero_wrapper.hpp>
#include <ze_ddi.h>

#if defined(_WIN32)
constexpr char l0_loader[] = "ze_loader.dll";
#elif defined(__linux__)
constexpr char l0_loader[] = "libze_loader.so.1";
#endif

namespace cassian {
LevelZeroWrapper::LevelZeroWrapper() {
  library_ = load_library(l0_loader);

  zeInit = reinterpret_cast<ze_pfnInit_t>(library_->get_function("zeInit"));
  zeDriverGet = reinterpret_cast<ze_pfnDriverGet_t>(
      library_->get_function("zeDriverGet"));
  zeDeviceGet = reinterpret_cast<ze_pfnDeviceGet_t>(
      library_->get_function("zeDeviceGet"));
  zeDeviceGetProperties = reinterpret_cast<ze_pfnDeviceGetProperties_t>(
      library_->get_function("zeDeviceGetProperties"));
  zeDeviceGetComputeProperties =
      reinterpret_cast<ze_pfnDeviceGetComputeProperties_t>(
          library_->get_function("zeDeviceGetComputeProperties"));
  zeDeviceGetModuleProperties =
      reinterpret_cast<ze_pfnDeviceGetModuleProperties_t>(
          library_->get_function("zeDeviceGetModuleProperties"));
  zeContextCreate = reinterpret_cast<ze_pfnContextCreate_t>(
      library_->get_function("zeContextCreate"));
  zeContextDestroy = reinterpret_cast<ze_pfnContextDestroy_t>(
      library_->get_function("zeContextDestroy"));
  zeCommandQueueCreate = reinterpret_cast<ze_pfnCommandQueueCreate_t>(
      library_->get_function("zeCommandQueueCreate"));
  zeCommandQueueDestroy = reinterpret_cast<ze_pfnCommandQueueDestroy_t>(
      library_->get_function("zeCommandQueueDestroy"));
  zeCommandQueueExecuteCommandLists =
      reinterpret_cast<ze_pfnCommandQueueExecuteCommandLists_t>(
          library_->get_function("zeCommandQueueExecuteCommandLists"));
  zeCommandQueueSynchronize = reinterpret_cast<ze_pfnCommandQueueSynchronize_t>(
      library_->get_function("zeCommandQueueSynchronize"));
  zeCommandListCreate = reinterpret_cast<ze_pfnCommandListCreate_t>(
      library_->get_function("zeCommandListCreate"));
  zeCommandListReset = reinterpret_cast<ze_pfnCommandListReset_t>(
      library_->get_function("zeCommandListReset"));
  zeCommandListDestroy = reinterpret_cast<ze_pfnCommandListDestroy_t>(
      library_->get_function("zeCommandListDestroy"));
  zeCommandListClose = reinterpret_cast<ze_pfnCommandListClose_t>(
      library_->get_function("zeCommandListClose"));
  zeModuleCreate = reinterpret_cast<ze_pfnModuleCreate_t>(
      library_->get_function("zeModuleCreate"));
  zeModuleDestroy = reinterpret_cast<ze_pfnModuleDestroy_t>(
      library_->get_function("zeModuleDestroy"));
  zeModuleDynamicLink = reinterpret_cast<ze_pfnModuleDynamicLink_t>(
      library_->get_function("zeModuleDynamicLink"));
  zeKernelCreate = reinterpret_cast<ze_pfnKernelCreate_t>(
      library_->get_function("zeKernelCreate"));
  zeKernelDestroy = reinterpret_cast<ze_pfnKernelDestroy_t>(
      library_->get_function("zeKernelDestroy"));
  zeKernelSetGroupSize = reinterpret_cast<ze_pfnKernelSetGroupSize_t>(
      library_->get_function("zeKernelSetGroupSize"));
  zeKernelSuggestGroupSize = reinterpret_cast<ze_pfnKernelSuggestGroupSize_t>(
      library_->get_function("zeKernelSuggestGroupSize"));
  zeKernelSetArgumentValue = reinterpret_cast<ze_pfnKernelSetArgumentValue_t>(
      library_->get_function("zeKernelSetArgumentValue"));
  zeCommandListAppendLaunchKernel =
      reinterpret_cast<ze_pfnCommandListAppendLaunchKernel_t>(
          library_->get_function("zeCommandListAppendLaunchKernel"));
  zeMemAllocShared = reinterpret_cast<ze_pfnMemAllocShared_t>(
      library_->get_function("zeMemAllocShared"));
  zeMemFree =
      reinterpret_cast<ze_pfnMemFree_t>(library_->get_function("zeMemFree"));
  zeCommandListAppendMemoryCopy =
      reinterpret_cast<ze_pfnCommandListAppendMemoryCopy_t>(
          library_->get_function("zeCommandListAppendMemoryCopy"));
  zeCommandListAppendImageCopyToMemory =
      reinterpret_cast<ze_pfnCommandListAppendImageCopyToMemory_t>(
          library_->get_function("zeCommandListAppendImageCopyToMemory"));
  zeCommandListAppendImageCopyFromMemory =
      reinterpret_cast<ze_pfnCommandListAppendImageCopyFromMemory_t>(
          library_->get_function("zeCommandListAppendImageCopyFromMemory"));
  zeImageCreate = reinterpret_cast<ze_pfnImageCreate_t>(
      library_->get_function("zeImageCreate"));
  zeImageDestroy = reinterpret_cast<ze_pfnImageDestroy_t>(
      library_->get_function("zeImageDestroy"));
  zeDeviceGetImageProperties =
      reinterpret_cast<ze_pfnDeviceGetImageProperties_t>(
          library_->get_function("zeDeviceGetImageProperties"));
  zeSamplerCreate = reinterpret_cast<ze_pfnSamplerCreate_t>(
      library_->get_function("zeSamplerCreate"));
  zeSamplerDestroy = reinterpret_cast<ze_pfnSamplerDestroy_t>(
      library_->get_function("zeSamplerDestroy"));
  zeModuleBuildLogGetString = reinterpret_cast<ze_pfnModuleBuildLogGetString_t>(
      library_->get_function("zeModuleBuildLogGetString"));
  zeModuleBuildLogDestroy = reinterpret_cast<ze_pfnModuleBuildLogDestroy_t>(
      library_->get_function("zeModuleBuildLogDestroy"));
  zeImageViewCreateExp = reinterpret_cast<ze_pfnImageViewCreateExp_t>(
      library_->get_function("zeImageViewCreateExp"));
  zeModuleGetNativeBinary = reinterpret_cast<ze_pfnModuleGetNativeBinary_t>(
      library_->get_function("zeModuleGetNativeBinary"));
  zeDeviceGetSubDevices = reinterpret_cast<ze_pfnDeviceGetSubDevices_t>(
      library_->get_function("zeDeviceGetSubDevices"));
}
} // namespace cassian
