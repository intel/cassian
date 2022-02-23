/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

enum class Alignment { DWALIGNED, MODIFIED_DWALIGNED, NOT_ALIGNED };

std::string dump_alignment(Alignment a) {
  std::string ans;
  switch (a) {
  case Alignment::DWALIGNED:
    ans = "BUF_DWALIGNED";
    break;
  case Alignment::MODIFIED_DWALIGNED:
    ans = "BUF_MODIFIED_DWALIGNED";
    break;
  case Alignment::NOT_ALIGNED:
    ans = "BUF_NOT_ALIGNED";
    break;
  }
  return ans;
}

template <typename T> std::string dump_vector(std::vector<T> vec) {
  std::ostringstream oss;
  constexpr bool fp = cassian::is_floating_point_v<T>;
  constexpr bool uns = std::is_unsigned_v<T>;
  using out_t = std::conditional_t<fp, double,
                                   std::conditional_t<uns, uint64_t, int64_t>>;
  if (fp) {
    oss.precision(std::numeric_limits<out_t>::max_digits10);
  }
  std::ostream_iterator<out_t> out(oss, fp ? "," : uns ? "ull," : "ll,");
  std::vector<out_t> vec_d(vec.begin(), vec.end());
  std::copy(vec_d.begin(), vec_d.end(), out);
  return oss.str();
}