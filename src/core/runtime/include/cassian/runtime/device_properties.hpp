/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_DEVICE_PROPERTIES_HPP
#define CASSIAN_RUNTIME_DEVICE_PROPERTIES_HPP

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Device properties.
 */
enum class DeviceProperty {
  max_total_group_size,
  max_group_size_x,
  max_group_size_y,
  max_group_size_z,
  max_num_samplers,
  image,
  image2d,
  max_local_memory_size,
  device_id,
};

} // namespace cassian
#endif
