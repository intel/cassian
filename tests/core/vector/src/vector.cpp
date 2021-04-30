/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <cassian/vector/vector.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Initializing Vector") {
  SECTION("with correct number of elements") {
    SECTION("from initializer list") {
      REQUIRE_NOTHROW(cassian::Vector<int, 4>{1, 2, 3, 4});
    }
    SECTION("from vector list") {
      std::vector<int> std_v = {1, 2, 3, 4};
      REQUIRE_NOTHROW(cassian::Vector<int, 4>(std_v));
    }
  }
  SECTION("with scalar") {
    const int value = 4;
    const cassian::Vector<int, 4> vector(value);
    for (size_t i = 0; i < vector.size(); ++i) {
      REQUIRE(vector[i] == value);
    }
  }
  SECTION("with too many of elements throws exception") {
    SECTION("from initializer list") {
      REQUIRE_THROWS_MATCHES((cassian::Vector<int, 4>{1, 2, 3, 4, 4}),
                             cassian::VectorBadNumberOfElementsException,
                             Catch::Message("cassian::Vector: initializer list "
                                            "contains 5 elements, expected 4"));
    }
    SECTION("from vector list") {
      std::vector<int> std_v = {1, 2, 3, 4, 4};
      REQUIRE_THROWS_MATCHES(
          (cassian::Vector<int, 4>(std_v)),
          cassian::VectorBadNumberOfElementsException,
          Catch::Message(
              "cassian::Vector: std::vector contains 5 elements, expected 4"));
    }
  }
  SECTION("with too few of elements throws exception") {
    SECTION("from initializer list") {
      REQUIRE_THROWS_MATCHES((cassian::Vector<int, 4>{1, 2, 3}),
                             cassian::VectorBadNumberOfElementsException,
                             Catch::Message("cassian::Vector: initializer list "
                                            "contains 3 elements, expected 4"));
    }
    SECTION("from vector list") {
      std::vector<int> std_v = {1, 2, 3};
      REQUIRE_THROWS_MATCHES(
          (cassian::Vector<int, 4>(std_v)),
          cassian::VectorBadNumberOfElementsException,
          Catch::Message(
              "cassian::Vector: std::vector contains 3 elements, expected 4"));
    }
  }
}

TEST_CASE("Access vector element") {
  cassian::Vector<int, 3, 4> v = {1, 2, 3};
  SECTION("within size") { REQUIRE_NOTHROW(v[2]); }

  SECTION("out of size") {
    REQUIRE_THROWS_MATCHES(
        v[3], cassian::VectorAccessViolationException,
        Catch::Message("cassian::Vector: access violation, accessed element: 3 "
                       "vector size: 3"));
  }
}

TEST_CASE("Equality operator") {
  SECTION("with the same vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    REQUIRE(a == a);
  }

  SECTION("with different vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {0, 1, 2};
    REQUIRE_FALSE(a == b);
  }

  SECTION("with partially different vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 5};
    REQUIRE_FALSE(a == b);
  }

  SECTION("with the same scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE(vector == scalar);
  }

  SECTION("with the same scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE(scalar == vector);
  }

  SECTION("with different scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(1);
    REQUIRE_FALSE(vector == scalar);
  }

  SECTION("with different scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(1);
    REQUIRE_FALSE(scalar == vector);
  }
}

TEST_CASE("Inequality operator") {
  SECTION("with the same vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    REQUIRE_FALSE(a != a);
  }

  SECTION("with different vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {0, 1, 2};
    REQUIRE(a != b);
  }

  SECTION("with partially different vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 5};
    REQUIRE(a != b);
  }

  SECTION("with the same scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE_FALSE(vector != scalar);
  }

  SECTION("with the same scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE_FALSE(scalar != vector);
  }

  SECTION("with different scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(1);
    REQUIRE(vector != scalar);
  }

  SECTION("with different scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(1);
    REQUIRE(scalar != vector);
  }
}

TEST_CASE("Less than operator") {
  SECTION("with the same vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    REQUIRE_FALSE(a < a);
  }

  SECTION("with lesser vector") {
    const cassian::Vector<int, 3, 4> a = {4, 5, 6};
    const cassian::Vector<int, 3, 4> b = {1, 2, 3};
    REQUIRE_FALSE(a < b);
  }

  SECTION("with greater vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 5, 6};
    REQUIRE(a < b);
  }

  SECTION("with partially greater vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 4};
    REQUIRE_FALSE(a < b);
  }

  SECTION("with partially lesser vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 2};
    REQUIRE_FALSE(a < b);
  }

  SECTION("with the same scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE_FALSE(vector < scalar);
  }

  SECTION("with the same scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE_FALSE(scalar < vector);
  }

  SECTION("with lesser scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(3);
    REQUIRE_FALSE(vector < scalar);
  }

  SECTION("with lesser scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(3);
    REQUIRE(scalar < vector);
  }

  SECTION("with greater scalar rhs") {
    const int scalar = 3;
    const cassian::Vector<int, 3, 4> vector(2);
    REQUIRE(vector < scalar);
  }

  SECTION("with greater scalar lhs") {
    const int scalar = 3;
    const cassian::Vector<int, 3, 4> vector(2);
    REQUIRE_FALSE(scalar < vector);
  }
}

TEST_CASE("Greater than operator") {
  SECTION("with the same vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    REQUIRE_FALSE(a > a);
  }

  SECTION("with lesser vector") {
    const cassian::Vector<int, 3, 4> a = {4, 5, 6};
    const cassian::Vector<int, 3, 4> b = {1, 2, 3};
    REQUIRE(a > b);
  }

  SECTION("with greater vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 5, 6};
    REQUIRE_FALSE(a > b);
  }

  SECTION("with partially greater vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 4};
    REQUIRE_FALSE(a > b);
  }

  SECTION("with partially lesser vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 2};
    REQUIRE_FALSE(a > b);
  }

  SECTION("with the same scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE_FALSE(vector > scalar);
  }

  SECTION("with the same scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE_FALSE(scalar > vector);
  }

  SECTION("with lesser scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(3);
    REQUIRE(vector > scalar);
  }

  SECTION("with lesser scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(3);
    REQUIRE_FALSE(scalar > vector);
  }

  SECTION("with greater scalar rhs") {
    const int scalar = 3;
    const cassian::Vector<int, 3, 4> vector(2);
    REQUIRE_FALSE(vector > scalar);
  }

  SECTION("with greater scalar lhs") {
    const int scalar = 3;
    const cassian::Vector<int, 3, 4> vector(2);
    REQUIRE(scalar > vector);
  }
}

TEST_CASE("Less than or equal operator") {
  SECTION("with the same vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    REQUIRE(a <= a);
  }

  SECTION("with lesser vector") {
    const cassian::Vector<int, 3, 4> a = {4, 5, 6};
    const cassian::Vector<int, 3, 4> b = {1, 2, 3};
    REQUIRE_FALSE(a <= b);
  }

  SECTION("with greater vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 5, 6};
    REQUIRE(a <= b);
  }

  SECTION("with partially greater vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 4};
    REQUIRE(a <= b);
  }

  SECTION("with partially lesser vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 2};
    REQUIRE_FALSE(a <= b);
  }

  SECTION("with the same scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE(vector <= scalar);
  }

  SECTION("with the same scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE(scalar <= vector);
  }

  SECTION("with lesser scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(3);
    REQUIRE_FALSE(vector <= scalar);
  }

  SECTION("with lesser scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(3);
    REQUIRE(scalar <= vector);
  }

  SECTION("with greater scalar rhs") {
    const int scalar = 3;
    const cassian::Vector<int, 3, 4> vector(2);
    REQUIRE(vector <= scalar);
  }

  SECTION("with greater scalar lhs") {
    const int scalar = 3;
    const cassian::Vector<int, 3, 4> vector(2);
    REQUIRE_FALSE(scalar <= vector);
  }
}

TEST_CASE("Greater than or equal operator") {
  SECTION("with the same vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    REQUIRE(a >= a);
  }

  SECTION("with lesser vector") {
    const cassian::Vector<int, 3, 4> a = {4, 5, 6};
    const cassian::Vector<int, 3, 4> b = {1, 2, 3};
    REQUIRE(a >= b);
  }

  SECTION("with greater vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 5, 6};
    REQUIRE_FALSE(a >= b);
  }

  SECTION("with partially greater vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 4};
    REQUIRE_FALSE(a >= b);
  }

  SECTION("with partially lesser vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {1, 2, 2};
    REQUIRE(a >= b);
  }

  SECTION("with the same scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE(vector >= scalar);
  }

  SECTION("with the same scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(scalar);
    REQUIRE(scalar >= vector);
  }

  SECTION("with lesser scalar rhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(3);
    REQUIRE(vector >= scalar);
  }

  SECTION("with lesser scalar lhs") {
    const int scalar = 2;
    const cassian::Vector<int, 3, 4> vector(3);
    REQUIRE_FALSE(scalar >= vector);
  }

  SECTION("with greater scalar rhs") {
    const int scalar = 3;
    const cassian::Vector<int, 3, 4> vector(2);
    REQUIRE_FALSE(vector >= scalar);
  }

  SECTION("with greater scalar lhs") {
    const int scalar = 3;
    const cassian::Vector<int, 3, 4> vector(2);
    REQUIRE(scalar >= vector);
  }
}

TEST_CASE("Addition assignment operator") {
  SECTION("with vector") {
    cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 5, 6};
    a += b;
    const cassian::Vector<int, 3, 4> expected = {5, 7, 9};
    REQUIRE(a == expected);
  }
}

TEST_CASE("Addition operator") {
  SECTION("with vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 5, 6};
    const cassian::Vector<int, 3, 4> output = a + b;
    const cassian::Vector<int, 3, 4> expected = {5, 7, 9};
    REQUIRE(output == expected);
  }

  SECTION("with scalar rhs") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const int b = 4;
    const cassian::Vector<int, 3, 4> output = a + b;
    const cassian::Vector<int, 3, 4> expected = {5, 6, 7};
    REQUIRE(output == expected);
  }

  SECTION("with scalar lhs") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const int b = 4;
    const cassian::Vector<int, 3, 4> output = b + a;
    const cassian::Vector<int, 3, 4> expected = {5, 6, 7};
    REQUIRE(output == expected);
  }
}

TEST_CASE("Subtraction assignment operator") {
  SECTION("with vector") {
    cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 6, 8};
    a -= b;
    const cassian::Vector<int, 3, 4> expected = {-3, -4, -5};
    REQUIRE(a == expected);
  }
}

TEST_CASE("Subtraction operator") {
  SECTION("with vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 6, 8};
    const cassian::Vector<int, 3, 4> output = a - b;
    const cassian::Vector<int, 3, 4> expected = {-3, -4, -5};
    REQUIRE(output == expected);
  }

  SECTION("with scalar rhs") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const int b = 4;
    const cassian::Vector<int, 3, 4> output = a - b;
    const cassian::Vector<int, 3, 4> expected = {-3, -2, -1};
    REQUIRE(output == expected);
  }

  SECTION("with scalar lhs") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const int b = 4;
    const cassian::Vector<int, 3, 4> output = b - a;
    const cassian::Vector<int, 3, 4> expected = {3, 2, 1};
    REQUIRE(output == expected);
  }
}

TEST_CASE("Multiplication operator") {
  SECTION("with vector") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const cassian::Vector<int, 3, 4> b = {4, 6, 8};
    const cassian::Vector<int, 3, 4> output = a * b;
    const cassian::Vector<int, 3, 4> expected = {4, 12, 24};
    REQUIRE(output == expected);
  }

  SECTION("with scalar rhs") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const int b = 4;
    const cassian::Vector<int, 3, 4> output = a * b;
    const cassian::Vector<int, 3, 4> expected = {4, 8, 12};
    REQUIRE(output == expected);
  }

  SECTION("with scalar lhs") {
    const cassian::Vector<int, 3, 4> a = {1, 2, 3};
    const int b = 4;
    const cassian::Vector<int, 3, 4> output = b * a;
    const cassian::Vector<int, 3, 4> expected = {4, 8, 12};
    REQUIRE(output == expected);
  }
}

TEST_CASE("Division operator") {
  SECTION("with vector") {
    const cassian::Vector<int, 3, 4> a = {4, 12, 24};
    const cassian::Vector<int, 3, 4> b = {4, 6, 8};
    const cassian::Vector<int, 3, 4> output = a / b;
    const cassian::Vector<int, 3, 4> expected = {1, 2, 3};
    REQUIRE(output == expected);
  }

  SECTION("with scalar rhs") {
    const cassian::Vector<int, 3, 4> a = {4, 8, 12};
    const int b = 4;
    const cassian::Vector<int, 3, 4> output = a / b;
    const cassian::Vector<int, 3, 4> expected = {1, 2, 3};
    REQUIRE(output == expected);
  }

  SECTION("with scalar lhs") {
    const cassian::Vector<int, 3, 4> a = {4, 3, 2};
    const int b = 12;
    const cassian::Vector<int, 3, 4> output = b / a;
    const cassian::Vector<int, 3, 4> expected = {3, 4, 6};
    REQUIRE(output == expected);
  }
}

TEST_CASE("Modulo operator") {
  SECTION("with vector") {
    const cassian::Vector<int, 3, 4> a = {4, 12, 24};
    const cassian::Vector<int, 3, 4> b = {3, 5, 7};
    const cassian::Vector<int, 3, 4> output = a % b;
    const cassian::Vector<int, 3, 4> expected = {1, 2, 3};
    REQUIRE(output == expected);
  }

  SECTION("with scalar rhs") {
    const cassian::Vector<int, 3, 4> a = {5, 10, 15};
    const int b = 4;
    const cassian::Vector<int, 3, 4> output = a % b;
    const cassian::Vector<int, 3, 4> expected = {1, 2, 3};
    REQUIRE(output == expected);
  }

  SECTION("with scalar lhs") {
    const cassian::Vector<int, 3, 4> a = {3, 4, 7};
    const int b = 15;
    const cassian::Vector<int, 3, 4> output = b % a;
    const cassian::Vector<int, 3, 4> expected = {0, 3, 1};
    REQUIRE(output == expected);
  }
}

TEST_CASE("Pre-increment operator") {
  const cassian::Vector<int, 3, 4> initial = {4, 12, 24};
  cassian::Vector<int, 3, 4> a = initial;
  const cassian::Vector<int, 3, 4> output = ++a;
  const cassian::Vector<int, 3, 4> expected = {5, 13, 25};
  REQUIRE(output == expected);
  REQUIRE(a == output);
}

TEST_CASE("Post-increment operator") {
  const cassian::Vector<int, 3, 4> initial = {4, 12, 24};
  cassian::Vector<int, 3, 4> a = initial;
  const cassian::Vector<int, 3, 4> output = a++;
  const cassian::Vector<int, 3, 4> expected = {5, 13, 25};
  REQUIRE(output == initial);
  REQUIRE(a == expected);
}

TEST_CASE("Pre-decrement operator") {
  const cassian::Vector<int, 3, 4> initial = {4, 12, 24};
  cassian::Vector<int, 3, 4> a = initial;
  const cassian::Vector<int, 3, 4> output = --a;
  const cassian::Vector<int, 3, 4> expected = {3, 11, 23};
  REQUIRE(output == expected);
  REQUIRE(a == output);
}

TEST_CASE("Post-decrement operator") {
  const cassian::Vector<int, 3, 4> initial = {4, 12, 24};
  cassian::Vector<int, 3, 4> a = initial;
  const cassian::Vector<int, 3, 4> output = a--;
  const cassian::Vector<int, 3, 4> expected = {3, 11, 23};
  REQUIRE(output == initial);
  REQUIRE(a == expected);
}

TEST_CASE("to_string") {
  SECTION("empty") {
    const cassian::Vector<int, 0> input = {};
    const std::string output = cassian::to_string(input);
    const std::string expected = "{}";
    REQUIRE(output == expected);
  }

  SECTION("single element") {
    const cassian::Vector<int, 1> input = {1};
    const std::string output = cassian::to_string(input);
    const std::string expected = "{1}";
    REQUIRE(output == expected);
  }

  SECTION("two elements") {
    const cassian::Vector<int, 2> input = {1, 2};
    const std::string output = cassian::to_string(input);
    const std::string expected = "{1, 2}";
    REQUIRE(output == expected);
  }

  SECTION("multiple elements") {
    const cassian::Vector<int, 3> input = {1, 2, 3};
    const std::string output = cassian::to_string(input);
    const std::string expected = "{1, 2, 3}";
    REQUIRE(output == expected);
  }
}

TEST_CASE("stream operator") {
  std::stringstream ss;

  SECTION("empty") {
    const cassian::Vector<int, 0> input = {};
    ss << input;
    const std::string expected = "{}";
    REQUIRE(ss.str() == expected);
  }

  SECTION("single element") {
    const cassian::Vector<int, 1> input = {1};
    ss << input;
    const std::string expected = "{1}";
    REQUIRE(ss.str() == expected);
  }

  SECTION("two elements") {
    const cassian::Vector<int, 2> input = {1, 2};
    ss << input;
    const std::string expected = "{1, 2}";
    REQUIRE(ss.str() == expected);
  }

  SECTION("multiple elements") {
    const cassian::Vector<int, 3> input = {1, 2, 3};
    ss << input;
    const std::string expected = "{1, 2, 3}";
    REQUIRE(ss.str() == expected);
  }
}

struct CustomType {
  using is_custom_type = int;
};

TEST_CASE("scalar_type_v") {
  SECTION("input scalar") {
    SECTION("int") {
      REQUIRE(std::is_same<cassian::scalar_type_v<int>, int>::value);
    }
    SECTION("float") {
      REQUIRE(std::is_same<cassian::scalar_type_v<float>, float>::value);
    }
    SECTION("custom type") {
      REQUIRE(
          std::is_same<cassian::scalar_type_v<CustomType>, CustomType>::value);
    }
  }
  SECTION("input vector") {
    SECTION("int2") {
      REQUIRE(std::is_same<cassian::scalar_type_v<cassian::Vector<int, 2>>,
                           int>::value);
    }
    SECTION("int4") {
      REQUIRE(std::is_same<cassian::scalar_type_v<cassian::Vector<int, 4>>,
                           int>::value);
    }
    SECTION("float2") {
      REQUIRE(std::is_same<cassian::scalar_type_v<cassian::Vector<float, 2>>,
                           float>::value);
    }
    SECTION("float4") {
      REQUIRE(std::is_same<cassian::scalar_type_v<cassian::Vector<float, 4>>,
                           float>::value);
    }
    SECTION("custom type 4") {
      REQUIRE(
          std::is_same<cassian::scalar_type_v<cassian::Vector<CustomType, 4>>,
                       CustomType>::value);
    }
  }
}

TEST_CASE("is_vector_v") {
  SECTION("input scalar") {
    SECTION("int") { REQUIRE_FALSE(cassian::is_vector_v<int>); }
    SECTION("float") { REQUIRE_FALSE(cassian::is_vector_v<float>); }
    SECTION("custom type") { REQUIRE_FALSE(cassian::is_vector_v<CustomType>); }
  }
  SECTION("input vector") {
    SECTION("int2") { REQUIRE(cassian::is_vector_v<cassian::Vector<int, 2>>); }
    SECTION("int4") { REQUIRE(cassian::is_vector_v<cassian::Vector<int, 4>>); }
    SECTION("float2") {
      REQUIRE(cassian::is_vector_v<cassian::Vector<float, 2>>);
    }
    SECTION("float4") {
      REQUIRE(cassian::is_vector_v<cassian::Vector<float, 4>>);
    }
    SECTION("custom type 4") {
      REQUIRE(cassian::is_vector_v<cassian::Vector<CustomType, 4>>);
    }
  }
}

template <typename T, typename cassian::EnableIfIsVector<T> = 0>
int function_a() {
  return 2;
}

template <typename T, typename cassian::EnableIfIsScalar<T> = 0>
int function_a() {
  return 4;
}

TEST_CASE("EnableIfIsVector EnablieIfIsScalar") {
  SECTION("scalar") {
    const int output = function_a<int>();
    const int expected = 4;
    REQUIRE(output == expected);
  }
  SECTION("vector") {
    const int output = function_a<cassian::Vector<int, 2>>();
    const int expected = 2;
    REQUIRE(output == expected);
  }
}

template <typename T, typename cassian::EnableIfIsIntegral<T> = 0>
int function_b() {
  return 2;
}

template <typename T, typename cassian::EnableIfIsFloatingPoint<T> = 0>
int function_b() {
  return 4;
}

TEST_CASE("EnableIfIsIntegral EnableIfIsFloatingPoint") {
  SECTION("integral") {
    const int output = function_b<int>();
    const int expected = 2;
    REQUIRE(output == expected);
  }
  SECTION("floating point") {
    const int output = function_b<float>();
    const int expected = 4;
    REQUIRE(output == expected);
  }
}

TEST_CASE("equal_component_wise") {
  SECTION("vector") {
    SECTION("equal") {
      const cassian::Vector<float, 2> lhs = {1, 2};
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, lhs);
      const cassian::Vector<int, 2> expected = {-1, -1};
      REQUIRE(output == expected);
    }
    SECTION("not equal") {
      const cassian::Vector<float, 2> lhs = {1, 2};
      const cassian::Vector<float, 2> rhs = {3, 4};
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {0, 0};
      REQUIRE(output == expected);
    }
    SECTION("partially equal") {
      const cassian::Vector<float, 2> lhs = {1, 2};
      const cassian::Vector<float, 2> rhs = {1, 4};
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {-1, 0};
      REQUIRE(output == expected);
    }
  }
  SECTION("scalar rhs") {
    SECTION("equal") {
      const cassian::Vector<float, 2> lhs = {2, 2};
      const float rhs = 2;
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {-1, -1};
      REQUIRE(output == expected);
    }
    SECTION("not equal") {
      const cassian::Vector<float, 2> lhs = {2, 2};
      const float rhs = 3;
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {0, 0};
      REQUIRE(output == expected);
    }
    SECTION("partially equal") {
      const cassian::Vector<float, 2> lhs = {2, 3};
      const float rhs = 2;
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {-1, 0};
      REQUIRE(output == expected);
    }
  }
  SECTION("scalar lhs") {
    SECTION("equal") {
      const float lhs = 2;
      const cassian::Vector<float, 2> rhs = {2, 2};
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {-1, -1};
      REQUIRE(output == expected);
    }
    SECTION("not equal") {
      const float lhs = 3;
      const cassian::Vector<float, 2> rhs = {2, 2};
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {0, 0};
      REQUIRE(output == expected);
    }
    SECTION("partially equal") {
      const float lhs = 2;
      const cassian::Vector<float, 2> rhs = {2, 3};
      const auto output =
          cassian::equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {-1, 0};
      REQUIRE(output == expected);
    }
  }
}

TEST_CASE("not_equal_component_wise") {
  SECTION("vector") {
    SECTION("equal") {
      const cassian::Vector<float, 2> lhs = {1, 2};
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, lhs);
      const cassian::Vector<int, 2> expected = {0, 0};
      REQUIRE(output == expected);
    }
    SECTION("not equal") {
      const cassian::Vector<float, 2> lhs = {1, 2};
      const cassian::Vector<float, 2> rhs = {3, 4};
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {-1, -1};
      REQUIRE(output == expected);
    }
    SECTION("partially equal") {
      const cassian::Vector<float, 2> lhs = {1, 2};
      const cassian::Vector<float, 2> rhs = {1, 4};
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {0, -1};
      REQUIRE(output == expected);
    }
  }
  SECTION("scalar rhs") {
    SECTION("equal") {
      const cassian::Vector<float, 2> lhs = {2, 2};
      const float rhs = 2;
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {0, 0};
      REQUIRE(output == expected);
    }
    SECTION("not equal") {
      const cassian::Vector<float, 2> lhs = {2, 2};
      const float rhs = 3;
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {-1, -1};
      REQUIRE(output == expected);
    }
    SECTION("partially equal") {
      const cassian::Vector<float, 2> lhs = {2, 3};
      const float rhs = 2;
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {0, -1};
      REQUIRE(output == expected);
    }
  }
  SECTION("scalar lhs") {
    SECTION("equal") {
      const float lhs = 2;
      const cassian::Vector<float, 2> rhs = {2, 2};
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {0, 0};
      REQUIRE(output == expected);
    }
    SECTION("not equal") {
      const float lhs = 3;
      const cassian::Vector<float, 2> rhs = {2, 2};
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {-1, -1};
      REQUIRE(output == expected);
    }
    SECTION("partially equal") {
      const float lhs = 2;
      const cassian::Vector<float, 2> rhs = {2, 3};
      const auto output =
          cassian::not_equal_component_wise<cassian::Vector<int, 2>>(lhs, rhs);
      const cassian::Vector<int, 2> expected = {0, -1};
      REQUIRE(output == expected);
    }
  }
}

TEST_CASE("not_component_wise") {
  SECTION("uniform non-zero") {
    const cassian::Vector<float, 2> input = {1, 2};
    const auto output =
        cassian::not_component_wise<cassian::Vector<int, 2>>(input);
    const cassian::Vector<int, 2> expected = {0, 0};
    REQUIRE(output == expected);
  }
  SECTION("uniform zero") {
    const cassian::Vector<float, 2> input = {0, 0};
    const auto output =
        cassian::not_component_wise<cassian::Vector<int, 2>>(input);
    const cassian::Vector<int, 2> expected = {-1, -1};
    REQUIRE(output == expected);
  }
  SECTION("partially zero") {
    const cassian::Vector<float, 2> input = {0, 2};
    const auto output =
        cassian::not_component_wise<cassian::Vector<int, 2>>(input);
    const cassian::Vector<int, 2> expected = {-1, 0};
    REQUIRE(output == expected);
  }
}

TEST_CASE("Unary minus operator") {
  const cassian::Vector<int, 3, 4> initial = {4, 12, 24};
  cassian::Vector<int, 3, 4> a = initial;
  const cassian::Vector<int, 3, 4> output = -a;
  const cassian::Vector<int, 3, 4> expected = {-4, -12, -24};
  REQUIRE(output == expected);
}