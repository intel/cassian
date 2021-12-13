/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <catch2/catch.hpp>

#include <cassian/main/flags_builder.hpp>
#include <cassian/main/test_helper.hpp>
#include <cassian/random/random.hpp>
#include <cassian/utility/utility.hpp>
#include <cassian/vector/vector.hpp>

#include <common.hpp>

TEST_CASE("cm_dp4(vector, vector)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector;
  InputType input_second = InputType::vector;
  SECTION("--no-sat") {
    test_dp<4>(1, 16, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(1, 16, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(1, 16, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector, vector_ref)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector;
  InputType input_second = InputType::vector_ref;
  SECTION("--no-sat") {
    test_dp<4>(1, 16, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(1, 16, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(1, 16, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector, matrix)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector;
  InputType input_second = InputType::matrix;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector, matrix_ref)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector;
  InputType input_second = InputType::matrix_ref;
  SECTION("--no-sat") {
    test_dp<4>(8, 2, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(8, 2, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(8, 2, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector, scalar)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector;
  InputType input_second = InputType::scalar;
  SECTION("--no-sat") {
    test_dp<4>(1, 16, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(1, 16, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(1, 16, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector, const)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector;
  InputType input_second = InputType::constant;
  SECTION("--no-sat") {
    test_dp<4>(1, 16, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(1, 16, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(1, 16, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector_ref, vector)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector_ref;
  InputType input_second = InputType::vector;
  SECTION("--no-sat") {
    test_dp<4>(1, 16, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(1, 16, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(1, 16, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector_ref, vector_ref)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector_ref;
  InputType input_second = InputType::vector_ref;
  SECTION("--no-sat") {
    test_dp<4>(1, 16, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(1, 16, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(1, 16, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector_ref, matrix)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector_ref;
  InputType input_second = InputType::matrix;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector_ref, matrix_ref)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector_ref;
  InputType input_second = InputType::matrix_ref;
  SECTION("--no-sat") {
    test_dp<4>(8, 2, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(8, 2, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(8, 2, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector_ref, scalar)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector_ref;
  InputType input_second = InputType::scalar;
  SECTION("--no-sat") {
    test_dp<4>(1, 16, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(1, 16, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(1, 16, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(vector_ref, const)", "[cm][dp][dp4]") {
  InputType input_first = InputType::vector_ref;
  InputType input_second = InputType::constant;
  SECTION("--no-sat") {
    test_dp<4>(1, 16, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(1, 16, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(1, 16, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix, vector)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix;
  InputType input_second = InputType::vector;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix, vector_ref)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix;
  InputType input_second = InputType::vector_ref;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix, matrix)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix;
  InputType input_second = InputType::matrix;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix, matrix_ref)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix;
  InputType input_second = InputType::matrix_ref;
  SECTION("--no-sat") {
    test_dp<4>(8, 2, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(8, 2, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(8, 2, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix, scalar)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix;
  InputType input_second = InputType::scalar;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix, const)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix;
  InputType input_second = InputType::constant;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix_ref, vector)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix_ref;
  InputType input_second = InputType::vector;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix_ref, vector_ref)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix_ref;
  InputType input_second = InputType::vector_ref;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix_ref, matrix)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix_ref;
  InputType input_second = InputType::matrix;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix_ref, matrix_ref)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix_ref;
  InputType input_second = InputType::matrix_ref;
  SECTION("--no-sat") {
    test_dp<4>(8, 2, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(8, 2, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(8, 2, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix_ref, scalar)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix_ref;
  InputType input_second = InputType::scalar;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}

TEST_CASE("cm_dp4(matrix_ref, const)", "[cm][dp][dp4]") {
  InputType input_first = InputType::matrix_ref;
  InputType input_second = InputType::constant;
  SECTION("--no-sat") {
    test_dp<4>(2, 8, -NO_SAT_LIM, NO_SAT_LIM, Saturation::NO_SAT, input_first,
               input_second);
  }
  SECTION("--sat-near-zero") {
    test_dp<4>(2, 8, -SAT_NEAR_ZERO_LIM, SAT_NEAR_ZERO_LIM,
               Saturation::USE_SAT_ZERO, input_first, input_second);
  }
  SECTION("--sat-no-limits") {
    test_dp<4>(2, 8, std::numeric_limits<float>::min(),
               std::numeric_limits<float>::max(), Saturation::USE_SAT_NOLIMITS,
               input_first, input_second);
  }
}