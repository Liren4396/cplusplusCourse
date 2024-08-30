#include "q2/record.h"
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("test empty record serialisation") {
	auto const empty_record = "{\n}\n";
	auto r = q2::record{};
	auto ss = std::stringstream();
	ss << r;
	CHECK(ss.str() == empty_record);
}
