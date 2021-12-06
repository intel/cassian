#include <catch2/catch.hpp>

#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/random/random.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

enum class Saturation { NO_SAT, USE_SAT_ZERO, USE_SAT_NOLIMITS };
enum class InputType {
  vector,
  matrix,
  vector_ref,
  matrix_ref,
  scalar,
  constant
};

const double NO_SAT_LEFT_LIM = 0.01;
const double NO_SAT_RIGHT_LIM = 100;
const double SAT_LEFT_LIM = -2;
const double SAT_RIGHT_LIM = 2;

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

template <typename T>
void test_atan2(int simd_u, int simd_v, std::string test_func,
                std::function<T(T, T)> f, float a, float b, Saturation sat_type,
                InputType input_type, double margin) {
  const std::string source =
      ca::load_text_file(ca::get_asset("kernels/cm_atan2/atan2_genx.cpp"));

  bool flag;
  std::vector<T> in0(8);
  std::vector<T> in1(8);
  std::string sat_flag;
  a = std::max(a, static_cast<float>(std::numeric_limits<T>::min()));
  b = std::min(b, static_cast<float>(std::numeric_limits<T>::max()));
  FlagsBuilder fb = FlagsBuilder(Language::cm);

  switch (sat_type) {
  case Saturation::NO_SAT:
    flag = false;
    sat_flag = "0";
    fb.define("SAT_FLAG", "0");
    break;
  default:
    flag = true;
    fb.define("SAT_FLAG", "SAT");
  }

  switch (input_type) {
  case InputType::vector: {
    in0 = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    in1 = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    ca::test::input(in0);
    ca::test::input(in1);
    fb.define("INPUT_VECTOR");
    fb.define("SIMD", std::to_string(simd_u * simd_v));
  } break;
  case InputType::vector_ref: {
    in0 = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    in1 = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    ca::test::input(in0);
    ca::test::input(in1);
    fb.define("INPUT_VECTOR_REF");
    fb.define("SIMD", std::to_string(simd_u * simd_v));
  } break;
  case InputType::matrix: {
    in0 = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    in1 = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    ca::test::input(in0);
    ca::test::input(in1);
    fb.define("INPUT_MATRIX");
    fb.define("SIMD_U", std::to_string(simd_u));
    fb.define("SIMD_V", std::to_string(simd_v));
  } break;
  case InputType::matrix_ref: {
    in0 = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    in1 = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    ca::test::input(in0);
    ca::test::input(in1);
    fb.define("INPUT_MATRIX_REF");
    fb.define("SIMD_U", std::to_string(simd_u));
    fb.define("SIMD_V", std::to_string(simd_v));
  } break;
  case InputType::scalar: {
    T in0_scalar = ca::generate_value<T>(T(a), T(b), 0);
    T in1_scalar = ca::generate_value<T>(T(a), T(b), 0);
    std::fill(in0.begin(), in0.end(), in0_scalar);
    std::fill(in1.begin(), in1.end(), in1_scalar);
    ca::test::pass(in0_scalar);
    ca::test::pass(in1_scalar);
    fb.define("INPUT_SCALAR");
  } break;
  case InputType::constant: {
    T in0_const = ca::generate_value<T>(T(a), T(b), 0);
    T in1_const = ca::generate_value<T>(T(a), T(b), 0);
    std::fill(in0.begin(), in0.end(), in0_const);
    std::fill(in1.begin(), in1.end(), in1_const);
    fb.define("INPUT0", std::to_string(static_cast<double>(in0_const)));
    fb.define("INPUT1", std::to_string(static_cast<double>(in1_const)));
    fb.define("INPUT_CONST");
  } break;
  }

  std::vector<T> ref;
  auto res = std::back_inserter(ref);
  std::transform(
      std::begin(in0), std::end(in0), std::begin(in1), res,
      [&flag, f](auto x, auto y) -> T { return saturate<T>(f(x, y), flag); });

  decltype(ref) out;

  ca::test::output(out, ref.size());

  ca::test::kernel("kernel", source,
                   fb.define("TEST_FUNC", test_func)
                       .define("IN_TYPE", ca::to_cm_string<T>())
                       .define("OUT_TYPE", ca::to_cm_string<T>())
                       .str());

  REQUIRE_THAT(out, Catch::Approx(ref).margin(static_cast<T>(margin)));
}
