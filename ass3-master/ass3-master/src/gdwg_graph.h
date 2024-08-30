#ifndef GDWG_GRAPH_H
#define GDWG_GRAPH_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <vector>

// This project implements a generic directed weighted graph (GDWG) with value semantics.
// It supports nodes of type N and edges with optional weights of type E.
// The graph is implemented using shared pointers and STL containers for efficient management of nodes and edges.
// The key features and implementation details are as follows:
//
// - Nodes are stored in a set with custom comparison for efficient lookup and ordering.
// - Edges are stored in a map with source nodes as keys and sets of edges as values.
// - Each edge is represented by a polymorphic class hierarchy, with separate classes for weighted and unweighted edges.
// - The graph supports basic operations like adding and removing nodes and edges, checking connectivity, and finding
// edges.
// - Iterator support is provided for traversing the edges in the graph.
// - The graph ensures strong exception safety and follows value semantics for copying and moving operations.
//
// Classes and Templates:
// - edge<N, E>: An abstract base class representing an edge with virtual methods for common operations.
// - weighted_edge<N, E>: A derived class for edges with weights, inheriting from edge<N, E>.
// - unweighted_edge<N, E>: A derived class for edges without weights, inheriting from edge<N, E>.
// - graph<N, E>: The main graph class template, providing the API for managing nodes and edges, and iterator support.
//
// Key Methods:
// - insert_node: Adds a node to the graph.
// - insert_edge: Adds an edge between two nodes, with an optional weight.
// - erase_node: Removes a node and all its edges from the graph.
// - erase_edge: Removes an edge between two nodes, optionally specifying the weight.
// - replace_node: Replaces a node with another node, updating all edges.
// - merge_replace_node: Merges two nodes, replacing one with the other and updating edges.
// - is_connected: Checks if there is an edge between two nodes.
// - nodes: Returns a list of all nodes in the graph.
// - edges: Returns a list of edges between two nodes.
// - connections: Returns a list of nodes connected to a given node.
//
// The graph also provides a custom iterator for traversing edges, supporting bidirectional iteration and comparison
// operations. The implementation ensures efficient management of nodes and edges, with appropriate handling of memory
// and exception safety.

namespace gdwg {
	template<typename N, typename E>
	class edge {
	 public:
		virtual ~edge() = default;
		virtual auto print_edge() const -> std::string = 0;
		virtual auto is_weighted() const -> bool = 0;
		virtual auto get_weight() const -> std::optional<E> = 0;
		virtual auto get_nodes() const -> std::pair<N, N> = 0;
		virtual void replace_node(const std::shared_ptr<N>& old_node, const std::shared_ptr<N>& new_node) = 0;
		virtual bool operator==(const edge<N, E>& other) const = 0;
	};
	template<typename N, typename E>
	class weighted_edge : public edge<N, E> {
	 public:
		weighted_edge(N src, N dst, E weight)
		: _src(std::move(src))
		, _dst(std::move(dst))
		, _weight(std::move(weight)) {}

		auto print_edge() const -> std::string override {
			std::ostringstream oss;
			oss << _src << " -> " << _dst << " | W | " << _weight;
			return oss.str();
		}

		auto is_weighted() const -> bool override {
			return true;
		}

		auto get_weight() const -> std::optional<E> override {
			return _weight;
		}

		auto get_nodes() const -> std::pair<N, N> override {
			return {_src, _dst};
		}

		auto replace_node(const std::shared_ptr<N>& old_node, const std::shared_ptr<N>& new_node) -> void override {
			if (_src == *old_node) {
				_src = *new_node;
			}
			if (_dst == *old_node) {
				_dst = *new_node;
			}
		}

		bool operator==(const edge<N, E>& other) const override {
			if (auto* other_edge = dynamic_cast<const weighted_edge*>(&other)) {
				return _src == other_edge->_src && _dst == other_edge->_dst && _weight == other_edge->_weight;
			}
			return false;
		}

	 private:
		N _src;
		N _dst;
		E _weight;
	};

	template<typename N, typename E>
	class unweighted_edge : public edge<N, E> {
	 public:
		unweighted_edge(N src, N dst)
		: _src(std::move(src))
		, _dst(std::move(dst)) {}

		auto print_edge() const -> std::string override {
			std::ostringstream oss;
			oss << _src << " -> " << _dst << " | U";
			return oss.str();
		}

		auto is_weighted() const -> bool override {
			return false;
		}

		auto get_weight() const -> std::optional<E> override {
			return std::nullopt;
		}

		auto get_nodes() const -> std::pair<N, N> override {
			return {_src, _dst};
		}

		auto replace_node(const std::shared_ptr<N>& old_node, const std::shared_ptr<N>& new_node) -> void override {
			if (_src == *old_node) {
				_src = *new_node;
			}
			if (_dst == *old_node) {
				_dst = *new_node;
			}
		}

		bool operator==(const edge<N, E>& other) const override {
			if (auto* other_edge = dynamic_cast<const unweighted_edge*>(&other)) {
				return _src == other_edge->_src && _dst == other_edge->_dst;
			}
			return false;
		}

	 private:
		N _src;
		N _dst;
	};

	template<typename N, typename E>
	class graph {
	 public:
		using edge_type = std::shared_ptr<edge<N, E>>;
		struct node_cmp {
			using is_transparent = void;
			bool operator()(const std::shared_ptr<N>& lhs, const std::shared_ptr<N>& rhs) const {
				return *lhs < *rhs;
			}
		};

		struct edge_cmp {
			bool operator()(const edge_type& lhs, const edge_type& rhs) const {
				auto lhs_nodes = lhs->get_nodes();
				auto rhs_nodes = rhs->get_nodes();
				if (lhs_nodes.first != rhs_nodes.first) {
					return lhs_nodes.first < rhs_nodes.first;
				}
				if (lhs->is_weighted() != rhs->is_weighted()) {
					return lhs->is_weighted() < rhs->is_weighted();
				}
				if (lhs_nodes.second != rhs_nodes.second) {
					return lhs_nodes.second < rhs_nodes.second;
				}
				if (lhs->is_weighted() && rhs->is_weighted()) {
					return lhs->get_weight().value() < rhs->get_weight().value();
				}

				return false;
			}
		};

		struct edge_list_cmp {
			using is_transparent = void;
			bool operator()(const std::shared_ptr<N>& lhs, const std::shared_ptr<N>& rhs) const {
				return *lhs < *rhs;
			}
		};

		graph()
		: _node(std::set<std::shared_ptr<N>, node_cmp>())
		, _edge(std::map<std::shared_ptr<N>, std::set<edge_type, edge_cmp>, edge_list_cmp>()) {}

		graph(std::initializer_list<N> il)
		: graph() {
			for (const auto& node : il) {
				_node.insert(std::make_shared<N>(node));
			}
		}

		template<typename InputIt>
		graph(InputIt first, InputIt last)
		: graph() {
			for (auto it = first; it != last; ++it) {
				_node.insert(std::make_shared<N>(*it));
			}
		}

		graph(graph&& other) noexcept
		: _node(std::move(other._node))
		, _edge(std::move(other._edge)) {}

		auto operator=(graph&& other) noexcept -> graph& {
			if (this != &other) {
				_node = std::move(other._node);
				_edge = std::move(other._edge);
			}
			return *this;
		}

		graph(graph const& other) {
			for (const auto& node : other._node) {
				_node.insert(std::make_shared<N>(*node));
			}
			for (const auto& [node, edges] : other._edge) {
				auto new_node = *(_node.find(node));
				for (const auto& edge : edges) {
					auto new_edge = edge;
					new_edge->replace_node(node, new_node);
					_edge[new_node].insert(new_edge);
				}
			}
		}

		auto operator=(graph const& other) -> graph& {
			if (this != &other) {
				_node.clear();
				_edge.clear();
				for (const auto& node : other._node) {
					_node.insert(std::make_shared<N>(*node));
				}
				for (const auto& [node, edges] : other._edge) {
					auto new_node = *(_node.find(node));
					for (const auto& edge : edges) {
						auto new_edge = edge;
						new_edge->replace_node(node, new_node);
						_edge[new_node].insert(new_edge);
					}
				}
			}
			return *this;
		}

		auto insert_node(N const& value) -> bool {
			auto node = std::make_shared<N>(value);
			return _node.insert(node).second;
		}

		[[nodiscard]] auto is_node(N const& value) const -> bool {
			auto node = std::make_shared<N>(value);
			return _node.find(node) != _node.end();
		}

		auto print_edge() const -> std::string {
			std::ostringstream oss;
			for (const auto& [src, edges] : _edge) {
				oss << *src << " (\n)";
				for (const auto& e : edges) {
					oss << e->print_edge() << "\n";
				}
				oss << *src << ")\n";
			}
			return oss.str();
		}

		auto is_weighted() const -> bool {
			for (const auto& [src, edges] : _edge) {
				for (const auto& e : edges) {
					if (e->is_weighted())
						return true;
				}
			}
			return false;
		}

		auto insert_edge(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::insert_edge when either src or dst node does "
				                         "not "
				                         "exist");
			}

			auto src_node = *std::find_if(_node.begin(), _node.end(), [&](const auto& n) { return *n == src; });
			auto dst_node = *std::find_if(_node.begin(), _node.end(), [&](const auto& n) { return *n == dst; });

			auto& edge_set = _edge[src_node];
			std::shared_ptr<edge<N, E>> new_edge;

			if (weight.has_value()) {
				new_edge = std::make_shared<weighted_edge<N, E>>(src, dst, weight.value());
			}
			else {
				new_edge = std::make_shared<unweighted_edge<N, E>>(src, dst);
			}

			for (const auto& e : edge_set) {
				if (*e == *new_edge) {
					return false;
				}
			}

			edge_set.insert(new_edge);
			return true;
		}

		auto replace_node(N const& old_data, N const& new_data) -> bool {
			if (!is_node(old_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::replace_node on a node that doesn't exist");
			}
			if (is_node(new_data)) {
				return false;
			}

			auto old_node_it = _node.find(std::make_shared<N>(old_data));
			if (old_node_it == _node.end()) {
				return false;
			}

			std::vector<edge_type> edges_to_update;
			for (const auto& [node, edges] : _edge) {
				for (const auto& edge : edges) {
					if (edge->get_nodes().first == old_data || edge->get_nodes().second == old_data) {
						edges_to_update.push_back(edge);
					}
				}
			}
			insert_node(new_data);
			for (const auto& edge : edges_to_update) {
				if (edge->get_nodes().first == old_data) {
					insert_edge(new_data, edge->get_nodes().second, edge->get_weight());
				}
				else {
					insert_edge(edge->get_nodes().first, new_data, edge->get_weight());
				}
				erase_edge(edge->get_nodes().first, edge->get_nodes().second, edge->get_weight());
			}
			erase_node(old_data);

			return true;
		}

		auto merge_replace_node(N const& old_data, N const& new_data) -> void {
			if (!is_node(old_data) || !is_node(new_data)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::merge_replace_node on old or new data if they "
				                         "don't exist in the graph");
			}

			auto old_node_ptr = std::make_shared<N>(old_data);
			auto new_node_ptr = std::make_shared<N>(new_data);

			std::set<edge_type, edge_cmp> edges_to_add;
			std::set<edge_type, edge_cmp> edges_to_remove;

			for (auto& edge : _edge[old_node_ptr]) {
				edge_type new_edge;
				if (edge->is_weighted()) {
					new_edge = std::make_shared<weighted_edge<N, E>>(new_data,
					                                                 edge->get_nodes().second,
					                                                 edge->get_weight().value());
				}
				else {
					new_edge = std::make_shared<unweighted_edge<N, E>>(new_data, edge->get_nodes().second);
				}
				edges_to_add.insert(new_edge);
				edges_to_remove.insert(edge);
			}

			for (auto& pair : _edge) {
				if (pair.first != old_node_ptr) {
					for (auto& edge : pair.second) {
						if (edge->get_nodes().second == old_data) {
							edge_type new_edge;
							if (edge->is_weighted()) {
								new_edge = std::make_shared<weighted_edge<N, E>>(edge->get_nodes().first,
								                                                 new_data,
								                                                 edge->get_weight().value());
							}
							else {
								new_edge = std::make_shared<unweighted_edge<N, E>>(edge->get_nodes().first, new_data);
							}
							edges_to_add.insert(new_edge);
							edges_to_remove.insert(edge);
						}
					}
				}
			}

			for (const auto& edge : edges_to_add) {
				_edge[std::make_shared<N>(edge->get_nodes().first)].insert(edge);
			}

			for (const auto& edge : edges_to_remove) {
				_edge[std::make_shared<N>(edge->get_nodes().first)].erase(edge);
			}

			_edge.erase(old_node_ptr);
			_node.erase(old_node_ptr);
			_node.insert(new_node_ptr);
		}

		auto erase_node(N const& value) -> bool {
			auto node_iter = _node.find(std::make_shared<N>(value));
			if (node_iter == _node.end()) {
				return false;
			}
			auto node_ptr = *node_iter;
			auto& edges = _edge[node_ptr];
			for (auto& edge_ptr : edges) {
				const auto& edge = *edge_ptr;
				auto [src_node, dst_node] = edge.get_nodes();

				if (src_node == value) {
					_edge[std::make_shared<N>(dst_node)].erase(edge_ptr);
				}
				else if (dst_node == value) {
					_edge[std::make_shared<N>(src_node)].erase(edge_ptr);
				}
			}
			_node.erase(node_iter);
			_edge.erase(node_ptr);

			return true;
		}

		auto erase_edge(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> bool {
			auto src_node = std::make_shared<N>(src);
			auto dst_node = std::make_shared<N>(dst);
			if (!is_node(src) || !is_node(dst))
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::erase_edge on src or dst if they don't exist "
				                         "in "
				                         "the graph");
			auto src_edges_iter = _edge.find(src_node);
			if (src_edges_iter == _edge.end())
				return false;
			auto& edges = src_edges_iter->second;
			bool erased = false;
			for (auto it = edges.begin(); it != edges.end();) {
				auto& edge_ptr = *it;
				const auto& edge = *edge_ptr;
				auto [edge_src, edge_dst] = edge.get_nodes();
				if (edge_src == src && edge_dst == dst) {
					if (weight.has_value()) {
						if (edge.get_weight() == weight.value()) {
							it = edges.erase(it);
							erased = true;
						}
						else {
							++it;
						}
					}
					else {
						it = edges.erase(it);
						erased = true;
					}
				}
				else {
					++it;
				}
			}
			if (erased) {
				auto& dst_edges = _edge[dst_node];
				for (auto it = dst_edges.begin(); it != dst_edges.end();) {
					auto& edge_ptr = *it;
					const auto& edge = *edge_ptr;
					auto [edge_src, edge_dst] = edge.get_nodes();
					if (edge_src == src && edge_dst == dst)
						it = dst_edges.erase(it);
					else
						++it;
				}
				if (_edge[src_node].empty()) {
					_edge.erase(src_node);
				}
				if (_edge[dst_node].empty()) {
					_edge.erase(dst_node);
				}
			}
			return erased;
		}

		[[nodiscard]] auto empty() noexcept -> bool {
			return _node.empty() && _edge.empty();
		}

		[[nodiscard]] auto is_connected(N const& src, N const& dst) -> bool {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::is_connected if src or dst node don't exist "
				                         "in "
				                         "the graph");
			}

			auto src_node_it = _node.find(std::make_shared<N>(src));
			if (src_node_it == _node.end()) {
				return false;
			}

			auto edges_it = _edge.find(std::make_shared<N>(src));
			if (edges_it != _edge.end()) {
				for (const auto& edge : edges_it->second) {
					if (edge->get_nodes().second == dst) {
						return true;
					}
				}
			}

			return false;
		}

		[[nodiscard]] auto nodes() -> std::vector<N> {
			std::vector<N> node_list;

			node_list.reserve(_node.size());

			for (const auto& node_ptr : _node) {
				node_list.push_back(*node_ptr);
			}

			return node_list;
		}

		[[nodiscard]] auto edges(N const& src, N const& dst) -> std::vector<std::shared_ptr<edge<N, E>>> {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the "
				                         "graph");
			}

			auto src_node_it = _node.find(std::make_shared<N>(src));
			if (src_node_it == _node.end()) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::edges if src or dst node don't exist in the "
				                         "graph");
			}

			std::vector<std::shared_ptr<edge<N, E>>> result;

			const auto& edges_from_src = _edge.at(*src_node_it);

			for (const auto& edge : edges_from_src) {
				if (edge->get_nodes().second == dst) {
					result.push_back(edge);
				}
			}

			std::sort(result.begin(),
			          result.end(),
			          [](const std::shared_ptr<edge<N, E>>& lhs, const std::shared_ptr<edge<N, E>>& rhs) {
				          return lhs->get_weight() < rhs->get_weight();
			          });

			return result;
		}

		[[nodiscard]] auto connections(N const& src) -> std::vector<N> {
			std::vector<N> connected_nodes;

			auto src_ptr = std::make_shared<N>(src);
			auto it = _edge.find(src_ptr);

			if (it != _edge.end()) {
				for (const auto& edge_ptr : it->second) {
					auto [edge_src, edge_dst] = edge_ptr->get_nodes();
					if (edge_dst != src) {
						connected_nodes.push_back(edge_dst);
					}
				}
			}

			return connected_nodes;
		}

		class iterator {
		 public:
			using value_type = struct {
				N from;
				N to;
				std::optional<E> weight;
			};
			using reference = value_type;
			using pointer = void;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::bidirectional_iterator_tag;

			iterator() = default;
			explicit iterator(
			    graph* g,
			    typename std::map<std::shared_ptr<N>, std::set<std::shared_ptr<edge<N, E>>, edge_cmp>, edge_list_cmp>::iterator
			        map_it,
			    typename std::set<std::shared_ptr<edge<N, E>>, edge_cmp>::iterator edge_it)
			: _graph(g)
			, _map_it(map_it)
			, _edge_it(edge_it) {}

			auto operator*() -> reference {
				auto edge = *_edge_it;

				return value_type{edge->get_nodes().first, edge->get_nodes().second, edge->get_weight()};
			}

			auto operator++() -> iterator& {
				++_edge_it;
				if (_edge_it == _map_it->second.end()) {
					++_map_it;
					if (_map_it != _graph->_edge.end()) {
						_edge_it = _map_it->second.begin();
					}
					else {
						_edge_it = typename std::set<std::shared_ptr<edge<N, E>>, edge_cmp>::iterator{};
					}
				}
				return *this;
			}

			auto operator++(int) -> iterator {
				auto tmp = *this;
				++(*this);
				return tmp;
			}

			auto operator--() -> iterator& {
				if (_edge_it == _map_it->second.begin()) {
					if (_map_it == _graph->_edge.begin()) {
						throw std::out_of_range("Iterator cannot be decremented beyond the start");
					}
					--_map_it;
					_edge_it = _map_it->second.end();
					--_edge_it;
				}
				else {
					--_edge_it;
				}
				return *this;
			}

			auto operator--(int) -> iterator {
				auto tmp = *this;
				--(*this);
				return tmp;
			}

			auto operator==(iterator const& other) const -> bool {
				return _map_it == other._map_it && _edge_it == other._edge_it;
			}

			auto operator!=(iterator const& other) const -> bool {
				return !(*this == other);
			}

			auto get_map() ->
			    typename std::map<std::shared_ptr<N>, std::set<std::shared_ptr<edge<N, E>>, edge_cmp>, edge_list_cmp>::iterator {
				return _map_it;
			}
			auto get_edge() -> typename std::set<std::shared_ptr<edge<N, E>>, edge_cmp>::iterator {
				return _edge_it;
			}
			friend auto operator&&(const iterator& lhs, const iterator& rhs) -> bool {
				return lhs._map_it == rhs._map_it && lhs._edge_it == rhs._edge_it;
			}

			friend auto operator||(const iterator& lhs, const iterator& rhs) -> bool {
				return lhs._map_it == rhs._map_it || lhs._edge_it == rhs._edge_it;
			}

		 private:
			graph* _graph;
			typename std::map<std::shared_ptr<N>, std::set<std::shared_ptr<edge<N, E>>, edge_cmp>, edge_list_cmp>::iterator _map_it;
			typename std::set<std::shared_ptr<edge<N, E>>, edge_cmp>::iterator _edge_it;
		};
		auto erase_edge(iterator i) -> iterator {
			if (i.get_map() == _edge.end()) {
				throw std::invalid_argument("Iterator must be valid");
			}

			auto map_it = i.get_map();
			auto edge_it = i.get_edge();

			map_it->second.erase(edge_it);

			auto next_map_it = map_it;
			if (map_it->second.empty()) {
				next_map_it = _edge.erase(map_it);
			}

			typename std::set<std::shared_ptr<edge<N, E>>, edge_cmp>::iterator new_edge_it;
			if (next_map_it != _edge.end()) {
				new_edge_it = next_map_it->second.begin();
			}
			else {
				new_edge_it = typename std::set<std::shared_ptr<edge<N, E>>, edge_cmp>::iterator();
			}

			return iterator(this, next_map_it, new_edge_it);
		}

		auto erase_edge(iterator i, iterator s) -> iterator {
			if (i.get_map() == _edge.end() || s.get_map() == _edge.end() || i == s) {
				return s;
			}

			auto map_it = i.get_map();
			auto edge_it = i.get_edge();
			auto end_map_it = s.get_map();
			auto end_edge_it = s.get_edge();

			while (map_it != end_map_it || (map_it == end_map_it && edge_it != end_edge_it)) {
				auto& edges = map_it->second;
				if (edge_it != edges.end()) {
					edge_it = edges.erase(edge_it);
				}
				else {
					++map_it;
					if (map_it != _edge.end()) {
						edge_it = map_it->second.begin();
					}
				}
			}

			if (map_it != _edge.end() && map_it->second.empty()) {
				_edge.erase(map_it);
			}

			if (map_it == _edge.end()) {
				return iterator(this, map_it, end_edge_it);
			}
			return iterator(this, map_it, edge_it);
		}

		[[nodiscard]] auto begin() -> iterator {
			return iterator{this, _edge.begin(), _edge.begin()->second.begin()};
		}

		[[nodiscard]] auto end() -> iterator {
			auto end_map_it = _edge.end();
			if (end_map_it == _edge.begin()) {
				return iterator{this, end_map_it, {}};
			}
			return iterator(this, _edge.end(), typename std::set<std::shared_ptr<edge<N, E>>, edge_cmp>::iterator{});
		}

		auto clear() noexcept -> void {
			_edge.clear();
			_node.clear();
			assert(empty());
		}

		[[nodiscard]] auto find(N const& src, N const& dst, std::optional<E> weight = std::nullopt) -> iterator {
			if (!is_node(src) || !is_node(dst)) {
				throw std::runtime_error("Cannot call gdwg::graph<N, E>::find if src or dst node don't exist in the "
				                         "graph");
			}

			auto src_node_it = _node.find(std::make_shared<N>(src));
			if (src_node_it == _node.end()) {
				return this->end();
			}

			auto edges_from_src_it = _edge.find(*src_node_it);
			if (edges_from_src_it == _edge.end()) {
				return this->end();
			}

			const auto& edges_from_src = edges_from_src_it->second;

			auto edge_it = std::find_if(edges_from_src.begin(),
			                            edges_from_src.end(),
			                            [&dst, &weight](const std::shared_ptr<edge<N, E>>& edge) {
				                            auto [edge_src, edge_dst] = edge->get_nodes();
				                            if (edge_dst != dst) {
					                            return false;
				                            }
				                            if (weight.has_value()) {
					                            return edge->is_weighted() && edge->get_weight() == weight;
				                            }
				                            else {
					                            return !edge->is_weighted();
				                            }
			                            });

			if (edge_it == edges_from_src.end()) {
				return this->end();
			}

			return iterator(this, edges_from_src_it, edge_it);
		}

		[[nodiscard]] auto operator==(graph const& other) const -> bool {
			if (_node.size() != other._node.size()) {
				return false;
			}
			for (const auto& node : _node) {
				bool found = false;
				for (const auto& other_node : other._node) {
					if (*node == *other_node) {
						found = true;
						break;
					}
				}
				if (!found) {
					return false;
				}
			}

			if (_edge.size() != other._edge.size()) {
				return false;
			}
			for (const auto& [node, edges] : _edge) {
				auto other_edges_it = other._edge.find(node);
				if (other_edges_it == other._edge.end()) {
					return false;
				}
				if (edges.size() != other_edges_it->second.size()) {
					return false;
				}
				for (const auto& e : edges) {
					bool found = false;
					for (const auto& oe : other_edges_it->second) {
						if (*e == *oe) {
							found = true;
							break;
						}
					}
					if (!found) {
						return false;
					}
				}
			}

			return true;
		}
		template<typename T, typename U>
		friend auto operator<<(std::ostream& os, graph<T, U> const& g) -> std::ostream&;

	 private:
		std::set<std::shared_ptr<N>, node_cmp> _node;
		std::map<std::shared_ptr<N>, std::set<edge_type, edge_cmp>, edge_list_cmp> _edge;
	};

	template<typename N, typename E>
	auto operator<<(std::ostream& os, graph<N, E> const& g) -> std::ostream& {
		os << '\n';
		for (const auto& node : g._node) {
			os << *node << " (\n";
			auto it = g._edge.find(node);
			if (it != g._edge.end()) {
				for (const auto& edge : it->second) {
					os << edge->print_edge() << "\n";
				}
			}
			os << ")\n";
		}
		return os;
	}

	template<typename N, typename E>
	auto operator<<(std::ostream& os, const edge<N, E>& e) -> std::ostream& {
		os << e.get_nodes().first << " -> " << e.get_nodes().second;
		if (e.is_weighted()) {
			os << " | " << *e.get_weight();
		}
		return os;
	}
} // namespace gdwg

#endif // GDWG_GRAPH_H
