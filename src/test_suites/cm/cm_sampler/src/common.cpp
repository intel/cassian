/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "common.hpp"

std::string to_cm_string(ChannelMask cmask) {
  std::string result = "CM_";

  if ((cmask & a) != 0) {
    result += "A";
  }
  if ((cmask & b) != 0) {
    result += "B";
  }
  if ((cmask & g) != 0) {
    result += "G";
  }
  if ((cmask & r) != 0) {
    result += "R";
  }

  result += "_ENABLE";

  return result;
}

const ChannelMask channel_masks[15] = {r,  g,  b,   a,   gr,  br,  ar,  bg,
                                       ag, ab, bgr, agr, abr, abg, abgr};
