#ifndef CASSIAN_CM_OPERATORS_COMMON_HPP
#define CASSIAN_CM_OPERATORS_COMMON_HPP

#include <cassert>
#include <cstddef>
#include <functional>
#include <iterator>
#include <string>
#include <variant>
#include <vector>

#include <cassian/fp_types/half.hpp>
#include <cassian/logging/logging.hpp>
#include <cassian/random/random.hpp>
#include <cassian/runtime/runtime.hpp>
#include <cassian/utility/metaprogramming.hpp>
#include <cassian/utility/utility.hpp>

#include <catch2/catch.hpp>

int suggest_work_size(const std::string &type);

template <class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

enum class OperandType {
  vector,
  vector_ref,
  matrix,
  matrix_ref,
  scalar,
  constant,
};

std::string to_compiler_flag(const OperandType op);

template <typename T> std::string to_cm_string(T value) {
  return std::to_string(value);
}

template <> std::string to_cm_string<int64_t>(int64_t value);
template <> std::string to_cm_string<uint32_t>(uint32_t value);
template <> std::string to_cm_string<uint64_t>(uint64_t value);
template <> std::string to_cm_string<float>(float value);
template <> std::string to_cm_string<cassian::Half>(cassian::Half value);

using arith_list_t =
    std::tuple<int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t,
               uint64_t, cassian::Half, float, double>;

using bitwise_list_t = std::tuple<int8_t, int16_t, int32_t, int64_t, uint8_t,
                                  uint16_t, uint32_t, uint64_t>;

template <typename KernelSource, typename Dst, typename... Srcs>
struct TestCase {
  using Generator =
      std::function<bool(int size, std::vector<Dst> &,
                         std::tuple<std::variant<std::vector<Srcs>, Srcs>...> &,
                         const std::array<OperandType, sizeof...(Srcs)> &)>;

  std::vector<Dst> dst;
  std::vector<Dst> ref;
  std::tuple<std::variant<std::vector<Srcs>, Srcs>...> srcs;

  bool generated;

  cassian::Runtime *runtime;
  cassian::Kernel kernel;

  static constexpr int threads = 1;
  static constexpr int simd = 16;
  static constexpr int size = threads * simd;

  std::vector<cassian::Buffer> buffers;

  TestCase(cassian::Runtime *rt, const std::string &program_type,
           const std::string &op, Generator generator, OperandType dst_operand,
           const std::array<OperandType, sizeof...(Srcs)> &src_operands,
           const std::vector<std::string> &extra_flags = {})
      : dst(), ref(), srcs(),
        generated(generator(size, ref, srcs, src_operands)), runtime(rt),
        kernel(create_kernel(program_type, op, dst_operand, src_operands,
                             extra_flags)) {
    assert(dst_operand == OperandType::vector);
    buffers.reserve(src_operands.size() + 1);

    int argument_index = 0;
    cassian::static_for(
        std::make_index_sequence<sizeof...(Srcs)>(), [&](auto i) {
          using Src = std::tuple_element_t<i.value, std::tuple<Srcs...>>;
          std::visit(
              overloaded{
                  [&](const std::vector<Src> &src) {
                    assert(src_operands[i.value] == OperandType::vector);
                    auto buffer =
                        runtime->create_buffer(src.size() * sizeof(Src));
                    runtime->write_buffer_from_vector(buffer, src);
                    runtime->set_kernel_argument(kernel, argument_index++,
                                                 buffer);
                    buffers.push_back(buffer);
                  },
                  [&](Src src) {
                    assert(src_operands[i.value] == OperandType::scalar ||
                           src_operands[i.value] == OperandType::constant);
                    if (src_operands[i.value] == OperandType::scalar) {
                      runtime->set_kernel_argument(kernel, argument_index++,
                                                   src);
                    }
                  }},
              std::get<i.value>(srcs));
        });

    auto buffer = runtime->create_buffer(size * sizeof(Dst));
    runtime->set_kernel_argument(kernel, argument_index++, buffer);
    buffers.push_back(buffer);

    runtime->run_kernel(kernel, threads);
    dst = runtime->read_buffer_to_vector<Dst>(buffer);
  }

  TestCase(const TestCase &) = delete;
  TestCase(TestCase &&) = delete;

  TestCase &operator=(const TestCase &) = delete;
  TestCase &operator=(TestCase &&) = delete;

  ~TestCase() {
    for (const auto &buffer : buffers) {
      runtime->release_buffer(buffer);
    }
    runtime->release_kernel(kernel);
  }

  void check() {
    if constexpr (cassian::is_floating_point_v<Dst>) {
      CHECK_THAT(dst, Catch::Approx(ref).epsilon(Dst(0.01)));
    } else {
      CHECK_THAT(dst, Catch::Equals(ref));
    }
  }

  cassian::Kernel
  create_kernel(const std::string &program_type, const std::string &op,
                OperandType dst_operand,
                const std::array<OperandType, sizeof...(Srcs)> &src_operands,
                const std::vector<std::string> extra_flags) {
    const auto &code = KernelSource::instance().source();

    std::vector<std::string> build_opts = {
        "-cmc",
        "-DSIMD=" + to_cm_string(simd),
        "-DOP=" + op,
        "-DDST_TYPE=\"" + cassian::to_cm_string<Dst>() + "\"",
        "-DDST_" + to_compiler_flag(dst_operand),
    };

    cassian::static_for(
        std::make_index_sequence<sizeof...(Srcs)>(), [&](auto i) {
          using Src = std::tuple_element_t<i.value, std::tuple<Srcs...>>;
          const auto op = src_operands[i];
          std::string prefix = "-DSRC" + std::to_string(i.value);
          build_opts.push_back(prefix + "_TYPE=\"" +
                               cassian::to_cm_string<Src>() + "\"");

          build_opts.push_back(prefix + "_" + to_compiler_flag(op));

          if (op == OperandType::constant) {
            using Src = std::tuple_element_t<i.value, std::tuple<Srcs...>>;
            auto value = std::get<Src>(std::get<i.value>(srcs));
            build_opts.push_back(prefix + "_VALUE=" + to_cm_string(value));
          }
        });

    std::ostringstream joined;
    std::copy(std::begin(build_opts), std::end(build_opts),
              std::ostream_iterator<std::string>(joined, " "));
    std::copy(std::begin(extra_flags), std::end(extra_flags),
              std::ostream_iterator<std::string>(joined, " "));

    cassian::logging::debug() << joined.str() << '\n';

    return runtime->create_kernel("test", code, joined.str(), program_type);
  }
};

template <typename S, typename D = decltype(+std::declval<S>())>
class UnaryVectorGenerator {
public:
  using src_t = S;
  using dst_t = D;

  static constexpr auto default_gen = []() {
    return cassian::generate_value<src_t>(0);
  };

  UnaryVectorGenerator(std::function<dst_t(src_t)> reference,
                       std::function<bool(src_t)> is_overflow,
                       std::function<src_t()> gen = default_gen)
      : reference_(reference), is_overflow_(is_overflow), gen_(gen) {}

  bool operator()(int size, std::vector<dst_t> &ref,
                  std::tuple<std::variant<std::vector<src_t>, src_t>> &srcs,
                  const std::array<OperandType, 1> &ops) {
    assert(ops[0] == OperandType::vector);

    auto &src = std::get<std::vector<src_t>>(std::get<0>(srcs));

    std::generate_n(std::back_inserter(src), size, [this, size]() {
      src_t s;

      int x = 0;
      do {
        s = gen_();
        assert(x++ < size * 100);
      } while (is_overflow_(s));

      return s;
    });

    std::transform(std::begin(src), std::end(src), std::back_inserter(ref),
                   reference_);
    return true;
  }

private:
  std::function<dst_t(src_t)> reference_;
  std::function<bool(src_t)> is_overflow_;
  std::function<src_t()> gen_;
};

template <typename S0, typename S1,
          typename D = decltype(std::declval<S0>() + std::declval<S1>())>
class VectorGenerator {
public:
  using src0_t = S0;
  using src1_t = S1;
  using dst_t = D;

  static constexpr auto default_g0 = []() {
    return cassian::generate_value<src0_t>(0);
  };
  static constexpr auto default_g1 = []() {
    return cassian::generate_value<src1_t>(0);
  };

  VectorGenerator(std::function<dst_t(src0_t, src1_t)> reference,
                  std::function<bool(src0_t, src1_t)> is_overflow,
                  std::function<src0_t()> gen0 = default_g0,
                  std::function<src1_t()> gen1 = default_g1)
      : reference_(reference), is_overflow_(is_overflow), gen0_(gen0),
        gen1_(gen1) {}

  bool operator()(int size, std::vector<dst_t> &ref,
                  std::tuple<std::variant<std::vector<src0_t>, src0_t>,
                             std::variant<std::vector<src1_t>, src1_t>> &srcs,
                  const std::array<OperandType, 2> &ops) {
    assert(ops[0] == OperandType::vector);
    assert(ops[1] == OperandType::vector);

    auto &src0 = std::get<std::vector<src0_t>>(std::get<0>(srcs));
    auto &src1 = std::get<std::vector<src1_t>>(std::get<1>(srcs));

    int x = 0;
    for (int i = 0; i < size; i++) {
      src0_t s0;
      src1_t s1;

      do {
        assert(x++ < size * 100);
        s0 = gen0_();
        s1 = gen1_();
      } while (is_overflow_(s0, s1));

      src0.push_back(s0);
      src1.push_back(s1);
    }

    std::transform(std::begin(src0), std::end(src0), std::begin(src1),
                   std::back_inserter(ref), reference_);
    return true;
  }

private:
  std::function<dst_t(src0_t, src1_t)> reference_;
  std::function<bool(src0_t, src1_t)> is_overflow_;
  std::function<src0_t()> gen0_;
  std::function<src1_t()> gen1_;
};

template <bool ScalarFirst, typename S0, typename S1,
          typename D = decltype(std::declval<S0>() + std::declval<S1>())>
class VectorScalarGenerator {
public:
  using src0_t = S0;
  using src1_t = S1;
  using dst_t = D;

  static constexpr auto default_g0 = []() {
    return cassian::generate_value<src0_t>(0);
  };
  static constexpr auto default_g1 = []() {
    return cassian::generate_value<src1_t>(0);
  };

  VectorScalarGenerator(std::function<dst_t(src0_t, src1_t)> reference,
                        std::function<bool(src0_t, src1_t)> is_overflow,
                        std::function<src0_t()> gen0 = default_g0,
                        std::function<src1_t()> gen1 = default_g1)
      : reference_(reference), is_overflow_(is_overflow), gen0_(gen0),
        gen1_(gen1) {}

  bool operator()(int size, std::vector<dst_t> &ref,
                  std::tuple<std::variant<std::vector<src0_t>, src0_t>,
                             std::variant<std::vector<src1_t>, src1_t>> &srcs,
                  const std::array<OperandType, 2> &ops) {

    using vector_t = std::conditional_t<ScalarFirst, src1_t, src0_t>;
    using scalar_t = std::conditional_t<ScalarFirst, src0_t, src1_t>;

    constexpr int vector_index = ScalarFirst ? 1 : 0;
    constexpr int scalar_index = ScalarFirst ? 0 : 1;

    auto &vector =
        std::get<std::vector<vector_t>>(std::get<vector_index>(srcs));
    scalar_t scalar;

    if constexpr (ScalarFirst) {
      scalar = gen0_();
    } else {
      scalar = gen1_();
    }

    std::get<scalar_index>(srcs) = scalar;

    int x = 0;
    std::generate_n(std::back_inserter(vector), size,
                    [this, scalar, size, &x]() mutable {
                      vector_t src;
                      if constexpr (ScalarFirst) {
                        do {
                          assert(x++ < size * 100);
                          src = gen1_();
                        } while (is_overflow_(scalar, src));
                      } else {
                        do {
                          assert(x++ < size * 100);
                          src = gen0_();
                        } while (is_overflow_(src, scalar));
                      }
                      return src;
                    });

    std::transform(std::begin(vector), std::end(vector),
                   std::back_inserter(ref), [this, scalar](auto src) {
                     dst_t res;
                     if constexpr (ScalarFirst) {
                       res = reference_(scalar, src);
                     } else {
                       res = reference_(src, scalar);
                     }
                     return res;
                   });

    return true;
  }

private:
  std::function<dst_t(src0_t, src1_t)> reference_;
  std::function<bool(src0_t, src1_t)> is_overflow_;
  std::function<src0_t()> gen0_;
  std::function<src1_t()> gen1_;
};

class Source {
public:
  const fs::path &path() const { return path_; }
  const std::string &source() const { return source_; }

protected:
  explicit Source(const std::string &filename)
      : path_(cassian::get_asset(filename)),
        source_(cassian::load_text_file(path_.string())) {}

  fs::path path_;
  std::string source_;
};

class UnarySource : public Source {
public:
  static UnarySource &instance();

private:
  UnarySource() : Source("kernels/cm_alu/unary_genx.cpp") {}
};

class UnaryIntrinsicSource : public Source {
public:
  static UnaryIntrinsicSource &instance();

private:
  UnaryIntrinsicSource() : Source("kernels/cm_alu/unary_intrinsic_genx.cpp") {}
};

class BinarySource : public Source {
public:
  static BinarySource &instance();

private:
  BinarySource() : Source("kernels/cm_alu/binary_genx.cpp") {}
};

class BinaryIntrinsicSource : public Source {
public:
  static BinaryIntrinsicSource &instance();

private:
  BinaryIntrinsicSource()
      : Source("kernels/cm_alu/binary_intrinsic_genx.cpp") {}
};

#endif
