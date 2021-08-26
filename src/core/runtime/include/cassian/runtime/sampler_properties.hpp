/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef CASSIAN_RUNTIME_SAMPLER_PROPERTIES_HPP
#define CASSIAN_RUNTIME_SAMPLER_PROPERTIES_HPP

/**
 * Cassian namespace.
 */
namespace cassian {

/**
 * Sampler Coordinates
 *
 * Specifies whether the image coordinates specified are normalized or not
 */
enum class SamplerCoordinates {
  unnormalized,
  normalized,
};

/**
 * Sampler Addressing Mode
 *
 * Specifies how out-of-range image coordinates are handled when reading from an
 * image
 */
enum class SamplerAddressingMode {
  none,          ///< Behavior is undefined for out-of-range image coordinates
  clamp_to_edge, ///< Out-of-range image coordinates are clamped to the edge of
                 ///< the image
  clamp,         ///< Out-of-range image coordinates are assigned a border color
  repeat,        ///< Out-of-range image coordinates read from the image as-if
                 ///< the image data were replicated in all dimensions
  mirror,        ///< Out-of-range image coordinates read from the image as-if
                 ///< the image data were replicated in all dimensions,
                 ///< mirroring the image contents at the edge of each
                 ///< replication
};

/**
 * Sample Filter Mode
 *
 * Specifies the type of filter that is applied when reading an image
 */
enum class SamplerFilterMode {
  nearest, ///< Returns the image element nearest to the image coordinate
  linear,  ///< Returns a weighted average of the four image elements nearest
           ///< to the image coordinate
};

} // namespace cassian
#endif
