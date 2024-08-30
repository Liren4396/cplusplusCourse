#include "sort3.h"
#include "catch.hpp"
TEST_CASE("swap a and b")
{
    auto a = 2, b = 1, c = 3;
    sort3(a, b, c);

    CHECK(a == 1);
    CHECK(b == 2);
    CHECK(c == 3);
}

TEST_CASE("sort a b c")
{
    auto a = 3, b = 2, c = 1;
    sort3(a, b, c);

    CHECK(a == 1);
    CHECK(b == 2);
    CHECK(c == 3);
}

TEST_CASE("sort string")
{
    std::string a = "3", b = "2", c = "1";
    sort3(a, b, c);

    CHECK(a == "1");
    CHECK(b == "2");
    CHECK(c == "3");
}