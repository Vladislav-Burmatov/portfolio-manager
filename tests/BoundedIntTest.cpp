#include <gtest/gtest.h>
#include <string>
#include <array>
#include "BoundedInt.h"

// Creates 3 different classes using BoundedInt template:
// - NormalBoundedInt can be initialised with any value in range [1, 100] including
//   the endpoints;
// - DegenerateBoundedInt has a range which has degenerated to a single point {1}, 
//   it can not be initialized with any value except 1;
// - InvalidBoundedInt has a range with lower_bound greater than the higher_bound; 
//   this range does not contain any numbers and InvalidBoundedInt member can not be 
//   initialized with any value without raising an exception.
using NormalBoundedInt = BoundedInt<1, 100>;
using DegenerateBoundedInt = BoundedInt<1, 1>;
using InvalidBoundedInt = BoundedInt<100, 1>;

// List of possible outcomes we may expect from a test
enum class ExpectedOutcome { Value, Exception };

// Structure containing all the data required to test the constructor of  
// Normal, Degenerate or Invalid BoundedInt. 
template <typename T>
struct TestCase_Constructor {
    int value;
    int expected_value;
    ExpectedOutcome outcome;
    std::string name;
};

// Function name PrintTo is expected by google test and shall not be changed
// Prints out the parameters of the constructor test as the test is run. For example: 
// (type=class BoundedInt<1,1>, name=TooSmall, value=0, expected=0, outcome=Exception) 
// Means that we were testing the constructor for BoundedInt<1, 1> (a.k.a. DegenerateBoundedInt), 
// the name of the test being ran is TooSmall,
// we tried to initialize DegenerateBoundedInt with the value 0 (which is too small for the range [1, 1])
// and expected this atempt to raise an exception.
template <typename T>
inline void PrintTo(const TestCase_Constructor<T>& c, std::ostream* os) {
    *os << "(type=" << typeid(T).name()
        << ", name=" << c.name
        << ", value=" << c.value
        << ", expected=" << c.expected_value
        << ", outcome=" << (c.outcome == ExpectedOutcome::Value ? "Value" : "Exception") << ") ";
}

template <typename T>
class BoundedIntTester_Constructor : public ::testing::TestWithParam<TestCase_Constructor<T>> {};

// Returns the name of the constructor test
template <typename T>
std::string CaseName_Constructor(const testing::TestParamInfo<TestCase_Constructor<T>>& info) {
    return info.param.name;
}

// Testing BoundedInt<1, 100> (NormalBoundedInt) initialization.
// Members of this class can be initialized with any value in range [1, 100],
// values on the boundaries of this range should not raise an exception.

using NormalTester_Constructor = BoundedIntTester_Constructor<NormalBoundedInt>;

TEST_P(NormalTester_Constructor, Constructor) {
    auto param = GetParam();
    if (param.outcome == ExpectedOutcome::Value) {
        NormalBoundedInt b(param.value);
        EXPECT_EQ(b.value(), param.expected_value);
    }
    else {
        EXPECT_THROW(NormalBoundedInt b(param.value), std::runtime_error);
    }
}

INSTANTIATE_TEST_SUITE_P(
    NormalCases,
    NormalTester_Constructor,
    ::testing::Values(
        TestCase_Constructor<NormalBoundedInt>{50, 50, ExpectedOutcome::Value, "InsideRange"},
        TestCase_Constructor<NormalBoundedInt>{1, 1, ExpectedOutcome::Value, "AtLowerBound"},
        TestCase_Constructor<NormalBoundedInt>{100, 100, ExpectedOutcome::Value, "AtUpperBound"},
        TestCase_Constructor<NormalBoundedInt>{0, 0, ExpectedOutcome::Exception, "BelowLower"},
        TestCase_Constructor<NormalBoundedInt>{101, 0, ExpectedOutcome::Exception, "AboveUpper"}
    ),
    CaseName_Constructor<NormalBoundedInt>
);

// Testing BoundedInt<1, 1> (DegenerateBoundedInt) initialization.
// Members of this class can only be initialized with value of 1, 
// any other value should raise an exception.

using DegenerateTester_Constructor = BoundedIntTester_Constructor<DegenerateBoundedInt>;

TEST_P(DegenerateTester_Constructor, Constructor) {
    auto param = GetParam();
    if (param.outcome == ExpectedOutcome::Value) {
        DegenerateBoundedInt b(param.value);
        EXPECT_EQ(b.value(), param.expected_value);
    }
    else {
        EXPECT_THROW(DegenerateBoundedInt b(param.value), std::runtime_error);
    }
}

INSTANTIATE_TEST_SUITE_P(
    DegenerateCases,
    DegenerateTester_Constructor,
    ::testing::Values(
        TestCase_Constructor<DegenerateBoundedInt>{1, 1, ExpectedOutcome::Value, "OnlyValidValue"},
        TestCase_Constructor<DegenerateBoundedInt>{0, 0, ExpectedOutcome::Exception, "TooSmall"},
        TestCase_Constructor<DegenerateBoundedInt>{2, 0, ExpectedOutcome::Exception, "TooLarge"}
    ),
    CaseName_Constructor<DegenerateBoundedInt>
);

// Testing BoundedInt<100, 1> (InvalidBoundedInt) initialization. 
// This class itself is invalid and attempting to initialize its 
// member with any value should raise an exception.

using InvalidTester_Constructor = BoundedIntTester_Constructor<InvalidBoundedInt>;

TEST_P(InvalidTester_Constructor, Constructor) {
    auto param = GetParam();
    EXPECT_THROW(InvalidBoundedInt b(param.value), std::runtime_error);
}

INSTANTIATE_TEST_SUITE_P(
    InvalidCases,
    InvalidTester_Constructor,
    ::testing::Values(
        TestCase_Constructor<InvalidBoundedInt>{1, 0, ExpectedOutcome::Exception, "AnyValueFails"}
    ),
    CaseName_Constructor<InvalidBoundedInt>
);

// Testing comparison operations between BoundedInt members.
// Comparison is dictated by the value_ field of Bounded int member.

struct TestCase_Comparison {
    int lhs_value;
    int rhs_value;
    std::array<bool, 6> expected_comparison_results;
    std::string name;
};

std::string CaseName_Comparison(const testing::TestParamInfo<TestCase_Comparison>& info) {
    return info.param.name;
}

class BoundedIntTester_Comparison : public ::testing::TestWithParam<TestCase_Comparison> {};

inline void PrintTo(const TestCase_Comparison& c, std::ostream* os) {
    *os << "(lhs=" << c.lhs_value << ", "
        << "rhs=" << c.rhs_value << ", "
        << "expected_results={ == : " << c.expected_comparison_results[0] << ", "
        << "!= : " << c.expected_comparison_results[1] << ", "
        << "< : " << c.expected_comparison_results[2] << ", "
        << "> : " << c.expected_comparison_results[3] << ", "
        << "<= : " << c.expected_comparison_results[4] << ", "
        << ">= : " << c.expected_comparison_results[5] << "} )";
}

TEST_P(BoundedIntTester_Comparison, Comparison) {
    auto param = GetParam();
    NormalBoundedInt lhs(param.lhs_value);
    NormalBoundedInt rhs(param.rhs_value);
    std::array<bool, 6> real_comparison_results = { lhs == rhs, lhs != rhs,
                                                 lhs < rhs, lhs > rhs,
                                                 lhs <= rhs, lhs >= rhs };
    EXPECT_EQ(real_comparison_results, param.expected_comparison_results);
}

INSTANTIATE_TEST_SUITE_P(
    Comparison,
    BoundedIntTester_Comparison,
    ::testing::Values(
        TestCase_Comparison{ 10, 20, {false, true, true, false, true, false}, "LhsLessThanRhs" },
        TestCase_Comparison{ 20, 20, {true, false, false, false, true, true}, "LhsEqualsRhs" },
        TestCase_Comparison{ 20, 10, {false, true, false, true, false, true}, "LhsGreaterThanRhs" }
    ),
    CaseName_Comparison
);


struct TestCase_Input {
    std::string input_string;
    int expected_value;
    ExpectedOutcome outcome;
    std::string name;
};

std::string CaseName_Input(const testing::TestParamInfo<TestCase_Input>& info) {
    return info.param.name;
}

class BoundedIntTester_Input : public ::testing::TestWithParam<TestCase_Input> {};

inline void PrintTo(const TestCase_Input& c, std::ostream* os) {
    *os << "(input_string=\"" << c.input_string << "\", "
        << "expected_value=" << c.expected_value << ", "
        << "expected_outcome=" << (c.outcome == ExpectedOutcome::Value ? "Value" : "Exception") << ", "
        << "name=" << c.name;
}

TEST_P(BoundedIntTester_Input, Input) {
    auto param = GetParam();
    std::istringstream iss(param.input_string);
    NormalBoundedInt b(1);

    if (param.outcome == ExpectedOutcome::Value) {
        iss >> b;
        EXPECT_FALSE(iss.fail());
        EXPECT_EQ(b.value(), param.expected_value);
    }
    else if (param.outcome == ExpectedOutcome::Exception) {
        EXPECT_TRUE(iss.fail());
    }
}