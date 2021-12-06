#include <catch2/catch.hpp>

#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/random/random.hpp>
#include <cassian/utility/utility.hpp>

namespace ca = cassian;
using ca::test::FlagsBuilder;
using ca::test::Language;

constexpr double pi = 3.1415926535897932384626433832795;
constexpr double trig_lim = 32767;

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

enum class Saturation { NO_SAT, USE_SAT_ZERO, USE_SAT_NOLIMITS };
enum class InputType { vector, matrix, scalar, constant };

const double NO_SAT_LEFT_LIM = 0.01;
const double NO_SAT_RIGHT_LIM = 100;
const double SAT_LEFT_LIM = -2;
const double SAT_RIGHT_LIM = 2;

template <typename T>
void test_math(int simd_u, int simd_v, std::string test_func,
               std::function<T(T)> f, float a, float b, Saturation sat_type,
               InputType input_type, double margin) {
  const std::string source =
      ca::load_text_file(ca::get_asset("kernels/cm_math/math_genx.cpp"));

  bool flag;
  std::vector<T> in(8);
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
    in = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    ca::test::input(in);
    fb.define("INPUT_VECTOR");
    fb.define("SIMD", std::to_string(simd_u * simd_v));
  } break;
  case InputType::matrix: {
    in = ca::generate_vector<T>(simd_u * simd_v, T(a), T(b), 0);
    ca::test::input(in);
    fb.define("INPUT_MATRIX");
    fb.define("SIMD_U", std::to_string(simd_u));
    fb.define("SIMD_V", std::to_string(simd_v));
  } break;
  case InputType::scalar: {
    T in_scalar = ca::generate_value<T>(T(a), T(b), 0);
    std::fill(in.begin(), in.end(), in_scalar);
    ca::test::pass(in_scalar);
    fb.define("INPUT_SCALAR");
  } break;
  case InputType::constant: {
    T in_const = ca::generate_value<T>(T(a), T(b), 0);
    std::fill(in.begin(), in.end(), in_const);
    fb.define("INPUT", std::to_string(static_cast<double>(in_const)));
    fb.define("INPUT_CONST");
  } break;
  }

  std::vector<T> ref;
  auto res = std::back_inserter(ref);
  std::transform(std::begin(in), std::end(in), res,
                 [&flag, f](auto x) { return saturate<T>(f(x), flag); });

  decltype(ref) out;

  ca::test::output(out, ref.size());

  ca::test::kernel("kernel", source,
                   fb.define("TEST_FUNC", test_func)
                       .define("IN_TYPE", ca::to_cm_string<T>())
                       .define("OUT_TYPE", ca::to_cm_string<T>())
                       .str());

  REQUIRE_THAT(out, Catch::Approx(ref).margin(static_cast<T>(margin)));
}
