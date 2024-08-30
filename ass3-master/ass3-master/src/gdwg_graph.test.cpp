#include "gdwg_graph.h"

#include <catch2/catch.hpp>

TEST_CASE("basic test") {
	auto g = gdwg::graph<int, std::string>{};
	auto n = 5;
	g.insert_node(n);
	CHECK(g.is_node(n));
}

TEST_CASE("Example test") {
	using graph = gdwg::graph<int, int>;
	auto const v = std::vector<std::tuple<int, int, std::optional<int>>>{
	    {4, 1, -4},
	    {3, 2, 2},
	    {2, 4, std::nullopt},
	    {2, 1, 1},
	    {6, 2, 5},
	    {6, 3, 10},
	    {1, 5, -1},
	    {3, 6, -8},
	    {4, 5, 3},
	    {5, 2, std::nullopt},
	};
	auto g = graph{};
	for (const auto& [from, to, weight] : v) {
		g.insert_node(from);
		g.insert_node(to);
		if (weight.has_value()) {
			g.insert_edge(from, to, weight.value());
		}
		else {
			g.insert_edge(from, to);
		}
	}
	g.insert_node(64);

	auto out = std::ostringstream{};
	out << g;
	auto const expected_output = std::string_view(R"(
1 (
1 -> 5 | W | -1
)
2 (
2 -> 4 | U
2 -> 1 | W | 1
)
3 (
3 -> 2 | W | 2
3 -> 6 | W | -8
)
4 (
4 -> 1 | W | -4
4 -> 5 | W | 3
)
5 (
5 -> 2 | U
)
6 (
6 -> 2 | W | 5
6 -> 3 | W | 10
)
64 (
)
)");
	CHECK(out.str() == expected_output);
}

TEST_CASE("Example1 test") {
	gdwg::graph<std::string, int> g;
	{
		std::string s1{"Hello"};
		g.insert_node(s1);
	}
	CHECK(true == g.is_node("Hello"));
}

TEST_CASE("Insert and Check Nodes") {
	gdwg::graph<int, int> g;
	CHECK_FALSE(g.is_node(10));
	g.insert_node(10);
	CHECK(g.is_node(10));
	CHECK_FALSE(g.is_node(20));
	g.insert_node(20);
	CHECK(g.is_node(20));
}

TEST_CASE("Insert and Check Edges") {
	gdwg::graph<int, std::string> g;
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);

	g.insert_edge(1, 2, "edge1");
	CHECK(g.is_connected(1, 2));
	CHECK_FALSE(g.is_connected(2, 1));
	CHECK_FALSE(g.is_connected(1, 3));
}

TEST_CASE("Remove Node and Edges") {
	gdwg::graph<int, int> g;
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);

	g.insert_edge(1, 2, 5);
	g.insert_edge(1, 3, 10);
	g.insert_edge(2, 3, 15);

	CHECK(g.is_connected(1, 2));
	g.erase_node(2);
	CHECK_FALSE(g.is_node(2));
	CHECK_THROWS_AS(g.is_connected(1, 2), std::runtime_error);
	CHECK_THROWS_AS(g.is_connected(2, 3), std::runtime_error);
}

TEST_CASE("Replace Node") {
	gdwg::graph<int, std::string> g;
	g.insert_node(1);
	g.insert_node(2);

	g.insert_edge(1, 2, "edge1");
	CHECK(g.is_connected(1, 2));
	g.replace_node(1, 3);
	CHECK_FALSE(g.is_node(1));
	CHECK(g.is_node(3));
	CHECK(g.is_connected(3, 2));
}

TEST_CASE("Clear Graph") {
	gdwg::graph<int, int> g;
	g.clear();

	bool exception_thrown = false;
	bool result = false;
	try {
		result = g.is_connected(1, 2);
	} catch (const std::runtime_error& e) {
		exception_thrown = true;
		REQUIRE(std::string(e.what())
		        == "Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist in the graph");
	} catch (...) {
		FAIL("Unexpected exception type thrown");
	}
	REQUIRE(result == false);
	REQUIRE(exception_thrown);
}

TEST_CASE("Output Stream Test") {
	gdwg::graph<int, int> g;
	g.insert_node(1);
	g.insert_node(2);
	g.insert_edge(1, 2, 5);

	auto out = std::ostringstream{};
	out << g;
	auto const expected_output = std::string_view(R"(
1 (
1 -> 2 | W | 5
)
2 (
)
)");
	CHECK(out.str() == expected_output);
}

TEST_CASE("Edge Iterators") {
	gdwg::graph<int, int> g;

	g.insert_node(1);
	g.insert_node(2);
	g.insert_edge(1, 2, 5);

	auto begin = g.begin();
	auto end = g.end();

	for (auto it = begin; it != end; ++it) {
		CHECK((*it).from == 1);
		CHECK((*it).to == 2);
		CHECK((*it).weight.value() == 5);
	}
}

TEST_CASE("Merge Nodes") {
	gdwg::graph<int, std::string> g;
	g.insert_node(1);
	g.insert_node(2);
	g.insert_node(3);

	g.insert_edge(1, 2, "edge1");
	g.insert_edge(2, 3, "edge2");
	g.merge_replace_node(2, 1);

	CHECK_FALSE(g.is_node(2));
	CHECK(g.is_node(1));
	CHECK(g.is_connected(1, 3));
	CHECK(g.is_connected(1, 1));
}

TEST_CASE("Erase Edge") {
	gdwg::graph<int, std::string> g;
	g.insert_node(1);
	g.insert_node(2);
	g.insert_edge(1, 2, "edge1");

	CHECK(g.is_connected(1, 2));
	g.erase_edge(1, 2, "edge1");
	CHECK_FALSE(g.is_connected(1, 2));
}

TEST_CASE("Insert Edge without Weight") {
	gdwg::graph<int, std::string> g;
	g.insert_node(1);
	g.insert_node(2);

	g.insert_edge(1, 2);
	CHECK(g.is_connected(1, 2));
	auto edges = g.edges(1, 2);
	CHECK(edges.size() == 1);
	CHECK_FALSE(edges[0]->is_weighted());
}

TEST_CASE("Find Edge") {
	gdwg::graph<int, int> g;
	g.insert_node(1);
	g.insert_node(2);
	g.insert_edge(1, 2, 5);

	auto it = g.find(1, 2, 5);
	CHECK(it != g.end());
	CHECK((*it).from == 1);
	CHECK((*it).to == 2);
	CHECK((*it).weight.value() == 5);
}

TEST_CASE("Graph Equality") {
	gdwg::graph<int, int> g1;
	gdwg::graph<int, int> g2;

	g1.insert_node(1);
	g1.insert_node(2);

	g2.insert_node(1);
	g2.insert_node(2);

	CHECK(g1 == g2);

	g2.insert_edge(2, 1, 10);
	CHECK_FALSE(g1 == g2);
}

TEST_CASE("Graph Copy and Move") {
	gdwg::graph<int, int> g1;
	g1.insert_node(1);
	g1.insert_node(2);
	g1.insert_edge(1, 2, 5);

	gdwg::graph<int, int> g2 = g1;
	CHECK(g1 == g2);

	gdwg::graph<int, int> g3 = std::move(g1);
	CHECK(g3.is_connected(1, 2));
	CHECK_THROWS_AS(g1.is_connected(1, 2), std::runtime_error);

	gdwg::graph<int, int> g4;
	g4 = g2;
	CHECK(g4 == g2);

	gdwg::graph<int, int> g5;
	g5 = std::move(g2);
	CHECK(g5.is_connected(1, 2));
	CHECK_THROWS_AS(g2.is_connected(1, 2), std::runtime_error);
}
