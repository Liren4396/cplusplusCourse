#include "setdiff.h"

#include "catch.hpp"

TEST_CASE("Empty vector unchanged") {
    auto vs = std::vector<char>{};
    auto blacklist = std::vector<char>{'o', 'k'};

    const auto expected = std::vector<char>{};

    set_difference(vs, blacklist);

    CHECK(vs == expected);
}

TEST_CASE("set_difference()")
{
    auto vs = std::vector<char>{'1', '2', '3', '4'};
    auto blacklist = std::vector<char> {'1', '2', '3', '4'};
    const auto expected = std::vector<char>{};
    set_difference(vs, blacklist);
    CHECK(vs == expected);
}

TEST_CASE("Empty blacklist ()")
{
    auto vs = std::vector<char>{'1', '2', '3', '4'};
    auto blacklist = std::vector<char> {};
    const auto expected = std::vector<char>{'1', '2', '3', '4'};
    set_difference(vs, blacklist);
    CHECK(vs == expected);
}
