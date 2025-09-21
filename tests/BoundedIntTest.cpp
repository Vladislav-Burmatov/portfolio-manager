#include <gtest/gtest.h>
#include <string>
#include "BoundedInt.h"

using NormalBoundedInt = BoundedInt<1, 100>;
using DegenerateBoundedInt = BoundedInt<1, 1>;
using InvalidBoundedInt = BoundedInt<100, 1>;

enum class ExpectedOutcome { Value, Exception };

template <typename T>
struct TestCase {
    int value;
    int expected_value;
    ExpectedOutcome outcome;
    std::string name;
};

template <typename T>
inline void PrintTo(const TestCase<T>& c, std::ostream* os) {
    *os << "(type=" << typeid(T).name()
        << ", name=" << c.name
        << ", value=" << c.value
        << ", expected=" << c.expected_value
        << ", outcome="
        << (c.outcome == ExpectedOutcome::Value ? "Value" : "Exception") << ") ";
}

template <typename T>
class BoundedIntTester : public ::testing::TestWithParam<TestCase<T>> {};

template <typename T>
std::string CaseName(const testing::TestParamInfo<TestCase<T>>& info) {
    return info.param.name;
}

using NormalTester = BoundedIntTester<NormalBoundedInt>;

TEST_P(NormalTester, Constructor) {
    auto param = GetParam();
    if (param.outcome == ExpectedOutcome::Value) {
        NormalBoundedInt b(param.value);
        EXPECT_EQ(b.value, param.expected_value);
    }
    else {
        EXPECT_THROW(NormalBoundedInt b(param.value), std::runtime_error);
    }
}

INSTANTIATE_TEST_SUITE_P(
    NormalCases,
    NormalTester,
    ::testing::Values(
        TestCase<NormalBoundedInt>{50, 50, ExpectedOutcome::Value, "InsideRange"},
        TestCase<NormalBoundedInt>{1, 1, ExpectedOutcome::Value, "AtLowerBound"},
        TestCase<NormalBoundedInt>{100, 100, ExpectedOutcome::Value, "AtUpperBound"},
        TestCase<NormalBoundedInt>{0, 0, ExpectedOutcome::Exception, "BelowLower"},
        TestCase<NormalBoundedInt>{101, 0, ExpectedOutcome::Exception, "AboveUpper"}
    ),
    CaseName<NormalBoundedInt>
);

using DegenerateTester = BoundedIntTester<DegenerateBoundedInt>;

TEST_P(DegenerateTester, Constructor) {
    auto param = GetParam();
    if (param.outcome == ExpectedOutcome::Value) {
        DegenerateBoundedInt b(param.value);
        EXPECT_EQ(b.value, param.expected_value);
    }
    else {
        EXPECT_THROW(DegenerateBoundedInt b(param.value), std::runtime_error);
    }
}

INSTANTIATE_TEST_SUITE_P(
    DegenerateCases,
    DegenerateTester,
    ::testing::Values(
        TestCase<DegenerateBoundedInt>{1, 1, ExpectedOutcome::Value, "OnlyValidValue"},
        TestCase<DegenerateBoundedInt>{0, 0, ExpectedOutcome::Exception, "TooSmall"},
        TestCase<DegenerateBoundedInt>{2, 0, ExpectedOutcome::Exception, "TooLarge"}
    ),
    CaseName<DegenerateBoundedInt>
);

using InvalidTester = BoundedIntTester<InvalidBoundedInt>;

TEST_P(InvalidTester, Constructor) {
    auto param = GetParam();
    EXPECT_THROW(InvalidBoundedInt b(param.value), std::runtime_error);
}

INSTANTIATE_TEST_SUITE_P(
    InvalidCases,
    InvalidTester,
    ::testing::Values(
        TestCase<InvalidBoundedInt>{1, 0, ExpectedOutcome::Exception, "AnyValueFails"}
    ),
    CaseName<InvalidBoundedInt>
);