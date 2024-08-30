#include "./filtered_string_view.h"

#include <catch2/catch.hpp>

TEST_CASE("Test Static Data Members") {
	for (char c = std::numeric_limits<char>::min(); c != std::numeric_limits<char>::max(); c++) {
		CHECK(fsv::filtered_string_view::default_predicate(c));
	}
}

TEST_CASE("Test Undefined Behaviour") {
	fsv::filtered_string_view sv("123");
	auto end = sv.end();
	end++;
}
TEST_CASE("Test empty string substr") {
	fsv::filtered_string_view sv("123");
	std::ostringstream x;
	x << fsv::substr(sv, 0, 0);
	const std::string expected = "";
	CHECK(x.str() == expected);
}
TEST_CASE("Test empty string -- begin and end") {
	fsv::filtered_string_view sv("");
	CHECK(sv.begin() == sv.end());
}
TEST_CASE("Test copy constructor") {
	auto fs1 = fsv::filtered_string_view{"hello"};
	auto copy = fsv::filtered_string_view{fs1};
	REQUIRE(fs1 == copy);
}
TEST_CASE("Test empty string == ") {
	auto const empty1 = fsv::filtered_string_view{};
	auto const empty2 = fsv::filtered_string_view{""};
	REQUIRE(empty1 == empty2);
}
TEST_CASE("Test empty string") {
	fsv::filtered_string_view sv("");
	CHECK(sv.size() == 0);
}
TEST_CASE("Test Default Constructor") {
	auto sv = fsv::filtered_string_view{};
	const std::size_t expected = 0;
	CHECK(sv.size() == expected);
	fsv::filtered_string_view* ptr = &sv;
	CHECK(ptr->size() == 0);
}

TEST_CASE("Test Implicit String Constructor") {
	auto s = std::string{"cat"};
	auto sv = fsv::filtered_string_view{s};
	const std::size_t expected = 3;
	CHECK(sv.size() == expected);
}

TEST_CASE("Test String Constructor with Predicate") {
	auto s = std::string{"cat"};
	auto pred = [](const char& c) { return c == 'a'; };
	auto sv = fsv::filtered_string_view{s, pred};
	const std::size_t expected = 1;
	CHECK(sv.size() == expected);
}

TEST_CASE("Test Implicit Null-Terminated String Constructor") {
	auto sv = fsv::filtered_string_view{"cat"};
	const std::size_t expected = 3;
	CHECK(sv.size() == expected);
}

TEST_CASE("Test Null-Terminated String with Predicate Constructor") {
	auto pred = [](const char& c) { return c == 'a'; };
	auto sv = fsv::filtered_string_view{"cat", pred};
	const std::size_t expected = 1;
	CHECK(sv.size() == expected);
}

TEST_CASE("Test Copy and Move Constructors") {
	auto sv1 = fsv::filtered_string_view{"bulldog"};
	const auto copy = sv1;
	CHECK(copy.data() == sv1.data()); // pointers compare equal.
	const auto move = std::move(sv1);
	CHECK(sv1.data() == nullptr); // true: sv1's guts were moved into `move`
}

TEST_CASE("Test Copy Assignment") {
	auto pred = [](const char& c) { return c == '4' || c == '2'; };
	auto fsv1 = fsv::filtered_string_view{"42 bro", pred};
	auto fsv2 = fsv::filtered_string_view{};
	fsv2 = fsv1;
	CHECK(fsv1 == fsv2);
}

TEST_CASE("Test Move Assignment") {
	auto pred = [](const char& c) { return c == '8' || c == '9'; };
	auto fsv1 = fsv::filtered_string_view{"'89 baby", pred};
	auto fsv2 = fsv::filtered_string_view{};
	fsv2 = std::move(fsv1);
	const std::size_t expected = 0;
	CHECK(fsv1.size() == expected);
	CHECK(fsv1.data() == nullptr);
}

TEST_CASE("Test Subscript") {
	auto pred = [](const char& c) { return c == '9' || c == '0' || c == ' '; };
	auto fsv1 = fsv::filtered_string_view{"only 90s kids understand", pred};
	const char expected = '0';
	CHECK(fsv1[2] == expected);
}

TEST_CASE("Test String Type Conversion") {
	auto sv = fsv::filtered_string_view("vizsla");
	auto s = static_cast<std::string>(sv);
	CHECK(sv.data() != s.data());
}

TEST_CASE("Test at method -- successful case") {
	auto vowels = std::set<char>{'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
	auto is_vowel = [&vowels](const char& c) { return vowels.contains(c); };
	auto sv = fsv::filtered_string_view{"Malamute", is_vowel};
	const char expected = 'a';
	CHECK(sv.at(0) == expected);
	CHECK(sv.at(1) == expected);
}

TEST_CASE("Test at method -- error throw case") {
	auto vowels = std::set<char>{'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
	auto is_vowel = [&vowels](const char& c) { return vowels.contains(c); };
	auto sv = fsv::filtered_string_view{"Malamute", is_vowel};

	const std::string expected1 = "filtered_string_view::at(5): invalid index";
	const std::string expected2 = "filtered_string_view::at(-1): invalid index";
	CHECK_THROWS_WITH(sv.at(5), expected1);
	CHECK_THROWS_WITH(sv.at(-1), expected2);
	sv = fsv::filtered_string_view{""};
	try {
		sv.at(0);
	} catch (const std::domain_error& e) {
		CHECK_THROWS_WITH(sv.at(0), "filtered_string_view::at(0): invalid index");
	}
}
TEST_CASE("Test at method in failed case") {
	auto sv = fsv::filtered_string_view{""};
	try {
		sv.at(0);
	} catch (const std::domain_error& e) {
		CHECK(std::string(e.what()) == "filtered_string_view::at(0): invalid index");
	}
}

TEST_CASE("Test size method with default predicate") {
	auto sv = fsv::filtered_string_view{"Maltese"};
	const std::size_t expected = 7;
	CHECK(sv.size() == expected);
}

TEST_CASE("Test size method with specified predicate") {
	auto sv = fsv::filtered_string_view{"Toy Poodle", [](const char& c) { return c == 'o'; }};
	const std::size_t expected = 3;
	CHECK(sv.size() == expected);
}

TEST_CASE("Test empty method with default predicate") {
	auto sv = fsv::filtered_string_view{"Australian Shephard"};
	auto empty_sv = fsv::filtered_string_view{};
	CHECK(sv.empty() == false);
	CHECK(empty_sv.empty() == true);
}

TEST_CASE("Test empty method with specified predicate") {
	auto sv = fsv::filtered_string_view{"Border Collie", [](const char& c) { return c == 'z'; }};
	CHECK(sv.empty() == true);
}

TEST_CASE("Test data method with specified predicate") {
	auto s = "Sum 42";
	auto sv = fsv::filtered_string_view{s, [](const char&) { return false; }};
	const std::string expected = "Sum 42";
	CHECK(std::string(sv.data()) == expected);
}

TEST_CASE("Test predicate method") {
	const auto print_and_return_true = [](const char&) {
		std::cout << "hi!";
		return true;
	};
	const auto s = fsv::filtered_string_view{"doggo", print_and_return_true};
	const auto& predicate = s.predicate();
	std::ostringstream oss;
	std::streambuf* old_cout_buf = std::cout.rdbuf(oss.rdbuf());
	CHECK(predicate(char{}));
	std::cout.rdbuf(old_cout_buf);
	const std::string expected = "hi!";
	CHECK(oss.str() == expected);
}

TEST_CASE("Test Equality Comparison") {
	auto const lo = fsv::filtered_string_view{"aaa"};
	auto const hi = fsv::filtered_string_view{"zzz"};
	CHECK(lo != hi);
	CHECK(!(lo == hi));
}

TEST_CASE("Test Relational Comparison") {
	auto const lo = fsv::filtered_string_view{"aaa"};
	auto const hi = fsv::filtered_string_view{"zzz"};
	CHECK((lo < hi) == true);
	CHECK((lo <= hi) == true);
	CHECK((lo > hi) == false);
	CHECK((lo >= hi) == false);
	CHECK((lo <=> hi) == std::strong_ordering::less);
}

TEST_CASE("Test Output Stream") {
	auto fsv = fsv::filtered_string_view{"c++ > rust > java", [](const char& c) { return c == 'c' || c == '+'; }};
	std::ostringstream x;
	x << fsv;
	const std::string expected = "c++";
	CHECK(x.str() == "c++");
}

TEST_CASE("Test compose method") {
	auto best_languages = fsv::filtered_string_view{"c / c++"};
	auto vf = std::vector<filter>{
	    [](const char& c) { return c == 'c' || c == '+' || c == '/'; },
	    [](const char& c) { return c > ' '; },
	};
	auto sv = fsv::compose(best_languages, vf);
	std::ostringstream x;
	x << sv;
	const std::string expected = "c/c++";
	CHECK(x.str() == expected);
}

TEST_CASE("Test split method") {
	auto interest = std::set<char>{'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F', ' ', '/'};
	auto sv = fsv::filtered_string_view{"0xDEADBEEF / 0xdeadbeef",
	                                    [&interest](const char& c) { return interest.contains(c); }};
	auto tok = fsv::filtered_string_view{" / "};
	auto v = fsv::split(sv, tok);
	std::ostringstream x;
	x << v[0] << " " << v[1];
	const std::string expected = "DEADBEEF deadbeef";
	CHECK(x.str() == expected);
}

TEST_CASE("Test split method --- front and end are delimiter example") {
	auto sv = fsv::filtered_string_view{"xax"};
	auto tok = fsv::filtered_string_view{"x"};
	auto v = fsv::split(sv, tok);
	const auto expected = std::vector<fsv::filtered_string_view>{"", "a", ""};
	CHECK(v == expected);
}

TEST_CASE("Test split method --- src string has the samme predicate with filter string") {
	const fsv::filtered_string_view sv = {"a/b/c", [](const char& c) { return c != '/'; }};
	const fsv::filtered_string_view tok = {"/"};
	std::vector<fsv::filtered_string_view> v = fsv::split(sv, tok);
	const auto expected = std::vector<fsv::filtered_string_view>{"abc"};
	CHECK(v.size() == 1);
	CHECK(v == expected);
}
TEST_CASE("Test split method --- all empty example") {
	auto sv = fsv::filtered_string_view{"xx"};
	auto tok = fsv::filtered_string_view{"x"};
	auto v = fsv::split(sv, tok);
	const auto expected = std::vector<fsv::filtered_string_view>{"", "", ""};
	CHECK(v == expected);
}

TEST_CASE("Test substr method --- default predicate") {
	auto sv = fsv::filtered_string_view{"Siberian Husky"};
	std::ostringstream x;
	x << fsv::substr(sv, 9);
	const std::string expected = "Husky";
	CHECK(x.str() == expected);
}

TEST_CASE("Test substr method --- upper predicate") {
	auto is_upper = [](const char& c) { return std::isupper(static_cast<unsigned char>(c)); };
	auto sv = fsv::filtered_string_view{"Sled Dog", is_upper};
	std::ostringstream x;
	x << fsv::substr(sv, 0, 2);
	const std::string expected = "SD";
	CHECK(x.str() == expected);
}

TEST_CASE("Test iterator --- print from begin to end") {
	std::ostringstream x;
	auto print_via_iterator = [](fsv::filtered_string_view const& sv, std::ostream& os) {
		std::copy(sv.begin(), sv.end(), std::ostream_iterator<char>(os, " "));
	};
	auto fsv1 = fsv::filtered_string_view{"corgi"};
	print_via_iterator(fsv1, x);
	const std::string expected = "c o r g i ";
	CHECK(x.str() == expected);
}

TEST_CASE("Test iterator --- next function") {
	auto fsv = fsv::filtered_string_view{"samoyed", [](const char& c) {
		                                     return !(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
	                                     }};
	auto it = fsv.begin();
	std::ostringstream x;
	x << *it << *std::next(it) << *std::next(it, 2) << *std::next(it, 3);
	const std::string expected = "smyd";
	CHECK(x.str() == expected);
}

TEST_CASE("Test iterator --- prev function") {
	const auto str = std::string("tosa");
	const auto s = fsv::filtered_string_view{str};
	auto it = s.cend();
	std::ostringstream x;
	x << *std::prev(it) << *std::prev(it, 2);
	const std::string expected = "as";
	CHECK(x.str() == expected);
}

TEST_CASE("Test Range --- ppp result") {
	const auto s = fsv::filtered_string_view{"puppy", [](const char& c) { return !(c == 'u' || c == 'y'); }};
	auto v = std::vector<char>{s.begin(), s.end()};
	std::ostringstream x;
	x << v[0] << v[1] << v[2];
	const std::string expected = "ppp";
	CHECK(x.str() == expected);
}

TEST_CASE("Test Range --- lm result") {
	auto s = fsv::filtered_string_view{"milo", [](const char& c) { return !(c == 'i' || c == 'o'); }};
	auto v = std::vector<char>{s.rbegin(), s.rend()};
	std::ostringstream x;
	x << v[0] << v[1];
	const std::string expected = "lm";
	CHECK(x.str() == expected);
}
