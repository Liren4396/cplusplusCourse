#include <array>
#include <string>

#include <catch2/catch.hpp>

#include "q2.h"

TEST_CASE("basic test") {
	auto start = std::array<std::array<q2::cell, 3>, 3>{
		{
			{ "0x2", "0x2", "" },
			{ "",    "",    "" },
			{ "",    "",    "" }
		}
	};

	q2::move_0x800(start, q2::Move::down);

	auto end = std::array<std::array<q2::cell, 3>, 3>{
		{
			{ "",    "",    "" },
			{ "",    "",    "" },
			{ "0x2", "0x2", "" }
		}
	};

	REQUIRE(start == end);
}
