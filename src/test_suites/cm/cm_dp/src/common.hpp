#include <catch2/catch.hpp>

#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/random/random.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

enum class Saturation { NO_SAT, USE_SAT_ZERO, USE_SAT_NOLIMITS };
enum class OperandOrder { first, second };
enum class InputType {
  vector,
  matrix,
  vector_ref,
  matrix_ref,
  scalar,
  constant
};

const float NO_SAT_LIM = 100;
const float SAT_NEAR_ZERO_LIM = 2;

std::string operand_to_flag(const OperandOrder order, const InputType type);

template <typename T> T saturate(T v, bool flag) {
  if (flag) {
    if (v >= 1)
      return T(1);
    if (v <= 0)
      return T(0);
    return v;
  }
  return v;
}

std::vector<ca::Vector<float, 4>> splitter(std::vector<float> input);

template <uint8_t N = 4>
std::vector<float> dp(const std::vector<float> &input_a,
                      const std::vector<float> &input_b) {
  std::vector<ca::Vector<float, 4>> input_a_vector = splitter(input_a);
  std::vector<ca::Vector<float, 4>> input_b_vector = splitter(input_b);

  std::vector<float> output_squeezed(input_a_vector.size(), 0);
  std::vector<float> output;
  for (size_t i = 0; i < output_squeezed.size(); ++i) {
    for (int j = 0; j < N; ++j) {
      output_squeezed[i] += input_a_vector[i][j] * input_b_vector[i][j];
    }
  }
  for (const float &value : output_squeezed) {
    fill_n(back_inserter(output), 4, value);
  }
  return output;
}

void intput_converter(OperandOrder operand, int simd_u, int simd_v, float a,
                      float b, InputType input_type, std::vector<float> &vec,
                      FlagsBuilder &fb);

template <uint8_t N>
void test_dp(int simd_u, int simd_v, float a, float b, Saturation sat_type,
             InputType first_input_type, InputType second_input_type) {
  const std::string source =
      ca::load_text_file(ca::get_asset("kernels/cm_dp/dp_genx.cpp"));

  bool inner_sat_flag;
  std::string outer_sat_flag;
  std::vector<float> in0(simd_u * simd_v);
  std::vector<float> in1(simd_u * simd_v);
  a = std::max(a, static_cast<float>(std::numeric_limits<float>::min()));
  b = std::min(b, static_cast<float>(std::numeric_limits<float>::max()));
  FlagsBuilder fb = FlagsBuilder(Language::cm);

  switch (sat_type) {
  case Saturation::NO_SAT:
    inner_sat_flag = false;
    outer_sat_flag = "0";
    fb.define("SAT_FLAG", "0");
    break;
  default:
    inner_sat_flag = true;
    fb.define("SAT_FLAG", "SAT");
  }

  intput_converter(OperandOrder::first, simd_u, simd_v, a, b, first_input_type,
                   in0, fb);
  intput_converter(OperandOrder::second, simd_u, simd_v, a, b,
                   second_input_type, in1, fb);

  std::vector<float> ref = dp<N>(in0, in1);
  std::transform(
      ref.begin(), ref.end(), ref.begin(),
      [&inner_sat_flag](auto x) { return saturate<float>(x, inner_sat_flag); });
  decltype(ref) out;

  ca::test::output(out, ref.size());

  ca::test::kernel("kernel", source,
                   fb.define("IN_TYPE", ca::to_cm_string<float>())
                       .define("OUT_TYPE", ca::to_cm_string<float>())
                       .define("TEST_FUNC", "cm_dp" + std::to_string(N))
                       .str());

  REQUIRE_THAT(out, Catch::Approx(ref));
}