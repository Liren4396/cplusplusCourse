#include "word_ladder.h"

#include <catch2/catch.hpp>

TEST_CASE("at -> it") {
	auto const lexicon = std::unordered_set<std::string>{
		"at",
		"it"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"at", "it"}
	};

	auto const ladders = word_ladder::generate("at", "it", lexicon);
	CHECK(ladders.size() != 0);
	CHECK(ladders.size() == 1);
	CHECK(ladders[0][0] == "at");
	CHECK(ladders[0][1] == "it");
	CHECK(ladders == expected);
}

TEST_CASE("dog -> hot (no connected word)") {
	auto const lexicon = std::unordered_set<std::string>{
		"dog",
		"hut",
		"hot"
	};

	const auto expected = std::vector<std::vector<std::string>>{
	};

	auto const ladders = word_ladder::generate("dog", "hot", lexicon);
	CHECK(ladders.size() == 0);
	CHECK(ladders == expected);
}

TEST_CASE("ab->cd (no connected word)") {
	auto const lexicon = std::unordered_set<std::string>{
		"ab",
		"cd"
	};

	const auto expected = std::vector<std::vector<std::string>>{
	};

	auto const ladders = word_ladder::generate("ab", "cd", lexicon);
	CHECK(ladders.size() == 0);
	CHECK(ladders == expected);
}

TEST_CASE("work -> play (only one way success)") {
	auto const lexicon = std::unordered_set<std::string>{
		"work", 
		"fork",
		"form",
		"foam",
		"flam",
		"flay", 
		"play"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"work", "fork", "form", "foam", "flam", "flay", "play"}
	};

	auto const ladders = word_ladder::generate("work", "play", lexicon);
	CHECK(ladders.size() == 1);
	CHECK(ladders[0].size() == 7);
	CHECK(ladders[0][1] == "fork");
	CHECK(ladders == expected);
}

TEST_CASE("work -> play (miss one word)") {
	auto const lexicon = std::unordered_set<std::string>{
		"work",
		"form",
		"foam",
		"flam",
		"flay", 
		"play"
	};

	const auto expected = std::vector<std::vector<std::string>>{
	};

	auto const ladders = word_ladder::generate("work", "play", lexicon);
	CHECK(ladders.size() == 0);
	CHECK(ladders == expected);
}

TEST_CASE("work -> play (dict with reversed order input)") {
	auto const lexicon = std::unordered_set<std::string>{
		"play",
		"flay", 
		"flam",
		"foam",
		"form",
		"fork",
		"work", 
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"work", "fork", "form", "foam", "flam", "flay", "play"}
	};
	auto const ladders = word_ladder::generate("work", "play", lexicon);
	CHECK(ladders.size() == 1);
	CHECK(ladders[0].size() == 7);
	CHECK(ladders[0][1] == "fork");
	CHECK(ladders == expected);
}

TEST_CASE("play -> work (one way success)") {
	auto const lexicon = std::unordered_set<std::string>{
		"work",
		"fork",
		"form",
		"foam",
		"flam",
		"flay", 
		"play"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"play", "flay", "flam", "foam", "form", "fork", "work"}
	};

	auto const ladders = word_ladder::generate("play", "work", lexicon);
	CHECK(ladders.size() == 1);
	CHECK(ladders[0].size() == 7);
	CHECK(ladders[0][1] == "flay");
	CHECK(ladders == expected);
}

TEST_CASE("work -> play (two ways success)") {
	auto const lexicon = std::unordered_set<std::string>{
		"work", 
		"fork",
		"form",
		"foam",
		"flam",
		"flay", 
		"play",
		"pork",
		"perk", 
		"peak", 
		"pean", 
		"plan"
	};

	const auto expected = std::vector<std::vector<std::string>>{
		{"work", "fork", "form", "foam", "flam", "flay", "play"},
		{"work", "pork", "perk", "peak", "pean", "plan", "play"}
	};

	auto const ladders = word_ladder::generate("work", "play", lexicon);
	CHECK(ladders.size() == 2);
	CHECK(ladders[0].size() == 7);
	CHECK(ladders[1].size() == 7);
	CHECK(ladders[0][2] == "form");
	CHECK(ladders[1][2] == "perk");
	CHECK(ladders == expected);
}

TEST_CASE("work -> play (12 ways success)") {
	auto const lexicon = ::word_ladder::read_lexicon("./english.txt");

	const auto expected = std::vector<std::vector<std::string>>{
		{ "work", "fork", "form", "foam", "flam", "flay", "play" }, 
		{ "work", "pork", "perk", "peak", "pean", "plan", "play" }, 
		{ "work", "pork", "perk", "peak", "peat", "plat", "play" }, 
		{ "work", "pork", "perk", "pert", "peat", "plat", "play" }, 
		{ "work", "pork", "porn", "pirn", "pian", "plan", "play" },
		{ "work", "pork", "port", "pert", "peat", "plat", "play" }, 
		{ "work", "word", "wood", "pood", "plod", "ploy", "play" }, 
  		{ "work", "worm", "form", "foam", "flam", "flay", "play" }, 
		{ "work", "worn", "porn", "pirn", "pian", "plan", "play" }, 
		{ "work", "wort", "bort", "boat", "blat", "plat", "play" }, 
		{ "work", "wort", "port", "pert", "peat", "plat", "play" }, 
		{ "work", "wort", "wert", "pert", "peat", "plat", "play" }

	};

	auto const ladders = word_ladder::generate("work", "play", lexicon);
	CHECK(ladders.size() == 12);
	for (std::size_t i = 0; i < 12; ++i) {
		CHECK(ladders[i].size() == 7);
		CHECK(ladders[i][3] == expected[i][3]);
	} 
	CHECK(ladders == expected);
}
