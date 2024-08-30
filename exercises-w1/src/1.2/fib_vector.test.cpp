#include "fib_vector.h"

#include "catch.hpp"

TEST_CASE("Works when n == 0")
{
    const auto expected = std::vector<int> {};
    auto nums = fibonacci(0);

    CHECK(nums == expected);
}

TEST_CASE("Works when n == 1")
{
    auto nums = fibonacci(1);

    CHECK(!nums.empty());
    CHECK(nums.size() == 1);

    CHECK(nums[0] == 1);
    CHECK(nums.at(0) == 1);
}

// add more tests here.

TEST_CASE("Works when n == 5")
{
    auto nums = fibonacci(5);

    CHECK(!nums.empty());
    CHECK(nums.size() == 5);

    CHECK(nums[0] == 1);
    CHECK(nums[1] == 2);
    CHECK(nums[2] == 3);
    CHECK(nums[3] == 5);
    CHECK(nums[4] == 8);
}