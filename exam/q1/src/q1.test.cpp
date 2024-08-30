#include <catch2/catch.hpp>

#include "q1.h"

TEST_CASE("comparing two smart lists") {
	auto l = q1::smart_list<int>(3);
	l.push_back(5);
	l.push_back(6);
	auto m = q1::smart_list<int>(3);
	m.push_back(5);
	m.push_back(6);
	auto ret = m + l;
	std::cout << ret;
	CHECK(l == m);
}
TEST_CASE("comparing two smart lists 1") {
	auto sl = q1::smart_list<double>{1,2,3};
	auto vsl = q1::very_smart_list<double>(sl);
	vsl = vsl * 3.0;
	CHECK(sl + vsl == q1::smart_list<double>{1,2,3,3,6,9});
}
TEST_CASE("comparing two smart lists 2") {
	auto a = q1::smart_list<double>{1,2,3};
	auto b = q1::smart_list<double>{2,3,4};
	auto c = a * b;
	CHECK(c == q1::smart_list<q1::smart_list<double>>{{2,3,4},{4,6,8},{6,9,12}});
}

TEST_CASE("comparing two smart lists 3") {
	auto l = q1::very_smart_list<int>{6,9,15};
	l = l / 3;
	CHECK(l == q1::very_smart_list<int>{2,3,5});
	std::cout << l;
	CHECK(l.is_prime());
}


TEST_CASE("comparing two smart lists 4") {
	auto a = q1::very_smart_list<double>{1,2,3};
	auto b = q1::very_smart_list<double>{2,3,4};
	auto c = a - b;
	CHECK(c == q1::very_smart_list<double>{1});
}
