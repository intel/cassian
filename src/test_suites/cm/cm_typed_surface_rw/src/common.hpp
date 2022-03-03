/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#include <catch2/catch.hpp>

#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/random/random.hpp>
#include <cassian/utility/utility.hpp>

#include <cassian/image/pixel/normalized.hpp>
#include <cassian/image/pixel/unnormalized.hpp>

#include <iterator>

using rgba_float =
    cassian::pixel::Unnormalized<float, cassian::ImageChannelOrder::rgba>;
using rgba_sint32 =
    cassian::pixel::Unnormalized<int32_t, cassian::ImageChannelOrder::rgba>;
using rgba_uint32 =
    cassian::pixel::Unnormalized<uint32_t, cassian::ImageChannelOrder::rgba>;

enum CMask {
  r = 1,
  g = 2,
  b = 4,
  a = 8,
  gr = r | g,
  br = r | b,
  ar = r | a,
  bg = g | b,
  ag = g | a,
  ab = b | a,
  bgr = r | g | b,
  agr = r | g | a,
  abr = r | b | a,
  abg = g | b | a,
  abgr = r | g | b | a,
};

std::string dump_cmask(CMask cmask);

const CMask channel_masks[] = {r,  g,  b,   a,   gr,  br,  ar,  bg,
                               ag, ab, bgr, agr, abr, abg, abgr};

template <typename T> std::string dump_vector(std::vector<T> vec) {
  std::ostringstream oss;
  constexpr bool fp = cassian::is_floating_point_v<T>;
  constexpr bool uns = std::is_unsigned_v<T>;
  using out_t = std::conditional_t<fp, double,
                                   std::conditional_t<uns, uint64_t, int64_t>>;
  if (fp) {
    oss.precision(std::numeric_limits<double>::max_digits10);
  }
  std::ostream_iterator<out_t> out(oss, fp ? "," : uns ? "ull," : "ll,");
  std::vector<out_t> vec_d(vec.begin(), vec.end());
  std::copy(vec_d.begin(), vec_d.end(), out);
  return oss.str();
}