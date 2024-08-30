#ifndef COMP6771_ASS2_FSV_H
#define COMP6771_ASS2_FSV_H

#include <cassert>
#include <compare>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <set>
#include <string>
using filter = std::function<bool(const char&)>;

/*               Design Concept
    The main difficulty for this task is to think & dicide which type of private member we need.
    Cause we need to construct a self_string type.
    So it becomes easy for me when I decide to use char* to construct self_string.
    And the task already given an iter class and its default function in class filtered_string_view
    that hasn't been implemented and In task web, it already given the all functions we need to implement.
    Lots of function's logic are very easy. Like when implement functions in class iter
    Just be careful when We need to use _predicate. And be careful dealing with boundary issues.
    In class filtered_string_view, I use smart pointers for
    automatic memory management that can avoid stack unwiding rather than using ref_count and new char.
*/

namespace fsv {
	class filtered_string_view;
	class iter;
	auto operator<<(std::ostream& os, const filtered_string_view& fsv) -> std::ostream&;
	auto operator<<(std::ostream& os, const iter& it) -> std::ostream&;
	auto operator==(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
	auto operator<(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
	auto operator<=(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
	auto operator>(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
	auto operator>=(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
	auto operator<=>(const filtered_string_view& left, const filtered_string_view& right) noexcept
	    -> std::strong_ordering;
	auto compose(const filtered_string_view& fsv, const std::vector<filter>& filts) noexcept -> filtered_string_view;
	auto split(const filtered_string_view& fsv, const filtered_string_view& tok) noexcept
	    -> std::vector<filtered_string_view>;
	auto substr(const filtered_string_view& fsv, int pos = 0, int count = 0) noexcept -> filtered_string_view;
	class filtered_string_view {
	 public:
		class iter {
		 public:
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = char;
			using difference_type = std::ptrdiff_t;
			using pointer = void;
			using reference = const char&;
			iter(char* ptr, filter pred) noexcept;
			auto operator*() const noexcept -> const char&;
			auto operator->() const noexcept -> const char*;
			auto operator++() noexcept -> iter&;
			auto operator++(int) noexcept -> iter;
			auto operator--() noexcept -> iter&;
			auto operator--(int) noexcept -> iter;
			friend auto operator==(const iter& left, const iter& right) noexcept -> bool;
			friend auto operator!=(const iter& left, const iter& right) noexcept -> bool;
			friend auto operator<<(std::ostream& os, const iter& it) -> std::ostream&;

		 private:
			char* _curr;
			filter _predicate;
		};
		using iterator = iter;
		using const_iterator = const iter;
		friend auto operator==(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
		friend auto operator<(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
		friend auto operator<=(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
		friend auto operator>(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
		friend auto operator>=(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool;
		friend auto operator<=>(const filtered_string_view& left, const filtered_string_view& right) noexcept
		    -> std::strong_ordering;
		auto operator=(const filtered_string_view& str) noexcept -> filtered_string_view&;
		auto operator=(filtered_string_view&& other) noexcept -> filtered_string_view&;
		auto operator[](std::size_t index) const noexcept -> char;
		explicit operator std::string() const noexcept;
		filtered_string_view() noexcept;
		filtered_string_view(const char* str) noexcept;
		filtered_string_view(const std::string& str) noexcept;
		filtered_string_view(const std::string& str, filter predicate) noexcept;
		filtered_string_view(const filtered_string_view& other) noexcept;
		filtered_string_view(filtered_string_view&& other) noexcept;
		~filtered_string_view() noexcept;
		static auto default_predicate(const char&) noexcept -> bool;
		auto begin() const noexcept -> iterator;
		auto end() const noexcept -> iterator;
		auto cbegin() const noexcept -> const_iterator;
		auto cend() const noexcept -> const_iterator;
		auto rbegin() noexcept -> std::reverse_iterator<iterator>;
		auto rend() noexcept -> std::reverse_iterator<iterator>;
		auto crbegin() const noexcept -> std::reverse_iterator<const_iterator>;
		auto crend() const noexcept -> std::reverse_iterator<const_iterator>;
		auto size() const noexcept -> std::size_t;
		auto data() const noexcept -> const char*;
		auto c_str() const noexcept -> const char*;
		auto at(int index) -> const char&;
		auto at(std::size_t index) const -> const char&;
		auto empty() noexcept -> bool;
		auto predicate() const noexcept -> const filter&;
		static auto predicate_str(const char* str, filter predicate) noexcept -> std::shared_ptr<char[]>;

	 private:
		std::shared_ptr<char[]> _str = nullptr;
		std::size_t _size = 0;
		filter _predicate = default_predicate;
	};
} // namespace fsv
#endif // COMP6771_ASS2_FSV_H
