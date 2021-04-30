/*
 *  Copyright (c) 2020 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 *
 *  Modifications by Intel Corporation (c) 2021.
 */

#ifndef CASSIAN_CATCH2_UTILS_CATCH2_UTILS_HPP
#define CASSIAN_CATCH2_UTILS_CATCH2_UTILS_HPP

#include <catch2/catch.hpp>

#define INTERNAL_CATCH_TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME_2(                  \
    TestName, TestFunc, Name, Tags, TmplList, NameFunc)                        \
  CATCH_INTERNAL_START_WARNINGS_SUPPRESSION                                    \
  CATCH_INTERNAL_SUPPRESS_GLOBALS_WARNINGS                                     \
  CATCH_INTERNAL_SUPPRESS_UNUSED_TEMPLATE_WARNINGS                             \
  template <typename TestType> static void TestFunc();                         \
  namespace {                                                                  \
  namespace INTERNAL_CATCH_MAKE_NAMESPACE(TestName) {                          \
    INTERNAL_CATCH_TYPE_GEN                                                    \
    template <typename... Types> struct TestName {                             \
      template <typename TestType> inline auto name_func() {                   \
        return NameFunc;                                                       \
      }                                                                        \
      void reg_tests() {                                                       \
        using expander = int[];                                                \
        (void)expander{                                                        \
            (Catch::AutoReg(                                                   \
                 Catch::makeTestInvoker(&TestFunc<Types>),                     \
                 CATCH_INTERNAL_LINEINFO, Catch::StringRef(),                  \
                 Catch::NameAndTags{Name " - " + name_func<Types>()(), Tags}), \
             0)...}; /* NOLINT */                                              \
      }                                                                        \
    };                                                                         \
    static int INTERNAL_CATCH_UNIQUE_NAME(globalRegistrar) = []() {            \
      using TestInit = typename convert<TestName, TmplList>::type;             \
      TestInit t;                                                              \
      t.reg_tests();                                                           \
      return 0;                                                                \
    }();                                                                       \
  }                                                                            \
  }                                                                            \
  CATCH_INTERNAL_STOP_WARNINGS_SUPPRESSION                                     \
  template <typename TestType> static void TestFunc()

#define INTERNAL_CATCH_TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(Name, Tags,         \
                                                           TmplList, NameFunc) \
  INTERNAL_CATCH_TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME_2(                        \
      INTERNAL_CATCH_UNIQUE_NAME(                                              \
          ____C_A_T_C_H____T_E_M_P_L_A_T_E____T_E_S_T____),                    \
      INTERNAL_CATCH_UNIQUE_NAME(                                              \
          ____C_A_T_C_H____T_E_M_P_L_A_T_E____T_E_S_T____F_U_N_C____),         \
      Name, Tags, TmplList, NameFunc)

#ifndef CATCH_CONFIG_TRADITIONAL_MSVC_PREPROCESSOR
#define TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(...)                               \
  INTERNAL_CATCH_TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(__VA_ARGS__)
#else
#define TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(...)                               \
  INTERNAL_CATCH_EXPAND_VARGS(                                                 \
      INTERNAL_CATCH_TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(__VA_ARGS__))
#endif

#endif