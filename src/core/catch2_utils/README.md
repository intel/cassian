# catch2_utils

## Overview

Extension of Catch2 framework.

## New macros

* **TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME(** _test name_, _tags_, _type list_, _name func_ **)**

_type list_ is a generic list of types on which test case should be instantiated.
List can be `std::tuple`, `boost::mpl::list`, `boost::mp11::mp_list` or anything with
`template <typename...>` signature.

This allows you to reuse the _type list_ in multiple test cases.

_name func_ is either name of a static function or a lambda that returns name as string.

TestType is available as a type to parametrize.

Function example:
```cpp
template <typename TestType> auto test_name() {
    TestType i = 1;
    return std::to_string(i);
}

using MyTypes = std::tuple<int, float>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("Template test case with test types specified inside std::tuple", "[template][list]", MyTypes, test_name<TestType>)
{
    REQUIRE(sizeof(TestType) > 0);
}
```

Lambda example:
```cpp
using MyTypes = std::tuple<int, float>;
TEMPLATE_LIST_TEST_CASE_CUSTOM_NAME("Template test case with test types specified inside std::tuple", "[template][list]", MyTypes, []() { return std::to_string(TestType(1)); })
{
    REQUIRE(sizeof(TestType) > 0);
}
```
