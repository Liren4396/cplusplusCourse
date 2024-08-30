#ifndef Q1_HPP_INCLUDED
#define Q1_HPP_INCLUDED

#include <cstddef>
#include <initializer_list>
#include <vector>
#include <iostream>
#include <ostream>
#include <algorithm>
namespace q1 {
	
	template <typename T>
	class smart_list {
	public:
		explicit smart_list(std::size_t count) {
			;
		}
		smart_list(T const& one, T const& two, std::size_t count) {
			if (one == two) throw("Cannot use multi-argument constructor with two identical elements");
			list = new std::vector<T>(count*2);
			for (int i = 0; i < count; ++i) {
				list.push_back(one);
				list.push_back(two);
			}
		}
		smart_list(std::initializer_list<T> il) {
			for (auto i : il) {
				list.push_back(i);
			}
		}
		// ... and more
		std::size_t size() {
			return list.size();
		}
		void push_back(const T& value) {
			list.push_back(value);
		}
		void pop_back() {
			list.pop_back();
		}
		T operator[](std::size_t index) {
			return list[index];
		}
		std::vector<T> list;
	};
	template <typename T>
	class smart_list;
	template <typename T>
	smart_list<T> operator+(smart_list<T> const& lhs, smart_list<T> const& rhs) {
		if (lhs.list.size() == 0 || rhs.list.size() == 0)
			throw("Cannot concatenate smart lists where one is empty");
		std::vector<T> ret;
		for (auto i : lhs.list)
			ret.push_back(i);
		for (auto i : rhs.list)
			ret.push_back(i);
		smart_list<T> ret_list = smart_list<T>(ret.size());
		for (auto i : ret) {
			ret_list.push_back(i);
		}
		return ret_list;
	}
	template <typename T>
	smart_list<T> operator-(smart_list<T> const& lhs, smart_list<T> const& Rhs) {
		if (lhs.list.size() == 0 || Rhs.list.size() == 0)
			throw("Cannot subtract smart lists where one is empty");
		
		smart_list<T> result(0);
		std::vector<bool> used(Rhs.list.size(), false);

		for (const auto& item : lhs.list) {
			auto it = std::find_if(Rhs.list.begin(), Rhs.list.end(),
								[&used, &Rhs, &item](const T& val) {
									return val == item && !used[&val - &Rhs.list[0]];
								});

			if (it == Rhs.list.end()) {
				result.list.push_back(item);
			} else {
				used[it - Rhs.list.begin()] = true;
			}
		}

		return result;
	}
	template <typename T>
	smart_list<smart_list<T>> operator*(smart_list<T> const& lhs, smart_list<T> const& rhs) {
		std::size_t n = rhs.list.size();
		smart_list<smart_list<T>> ret(0);
		for (int i = 0; i < n; ++i) {
			smart_list<T> tmp(n);
			for (auto j : rhs.list) {
				tmp.push_back(j*lhs.list[i]);
			}
			ret.push_back(tmp);
		}
		return ret;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, smart_list<T> const& sl) {
		for (auto i : sl.list) {
			os << "|" << i;
		}
		os << "|";
		return os;
	}
	template <typename T>
	bool operator==(smart_list<T> const& lhs, smart_list<T> const& rhs) {
		if (lhs.list.size() != rhs.list.size()) return false;
		int n = lhs.list.size();
		for (int i = 0; i < n; ++i) {
			if (lhs.list[i] != rhs.list[i]) return false;
		}
		return true;
	}
	template <typename T>
	class very_smart_list {
	public:
		explicit very_smart_list(std::size_t count) {}
		explicit very_smart_list(smart_list<T> const& other) {
			for (auto i : other.list) list.push_back(i);
		}
		very_smart_list(std::initializer_list<T> il) {
			for (auto i : il) {
				list.push_back(i);
			}
		}
		explicit very_smart_list(smart_list<T>&& other) {
			for (auto i : other.list) {
				list.push_back(i);
			}
			other = nullptr;
		}
		std::size_t size() {
			return list.size();
		}
		void push_back(const T& value) {
			list.push_back(value);
		}
		void pop_back() {
			list.pop_back();
		}
		T operator[](std::size_t index) {
			return list[index];
		}
		
		bool is_prime() {
			for (auto i : list) {
				if (i != static_cast<int>(i)) {
					return false;
				} else if (i < 1) {
					return false;
				}
			}
			return true;
		}
		std::vector<T> list;
	};
	template <typename T>
	class very_smart_list;
	template <typename T>
	very_smart_list<T> operator+(very_smart_list<T> const& lhs, very_smart_list<T> const& rhs) {
		if (lhs.list.size() == 0 || rhs.list.size() == 0)
			throw("Cannot concatenate smart lists where one is empty");
		std::vector<T> ret;
		for (auto i : lhs.list)
			ret.push_back(i);
		for (auto i : rhs.list)
			ret.push_back(i);
		very_smart_list<T> ret_list = very_smart_list<T>(ret.size());
		for (auto i : ret) {
			ret_list.push_back(i);
		}
		return ret_list;
	}
	template <typename T>
	smart_list<T> operator+(smart_list<T> const& lhs, very_smart_list<T> const& rhs) {
		if (lhs.list.size() == 0 || rhs.list.size() == 0)
			throw("Cannot concatenate smart lists where one is empty");
		std::vector<T> ret;
		for (auto i : lhs.list)
			ret.push_back(i);
		for (auto i : rhs.list)
			ret.push_back(i);
		smart_list<T> ret_list = smart_list<T>(ret.size());
		for (auto i : ret) {
			ret_list.push_back(i);
		}
		return ret_list;
	}
	template <typename T>
	very_smart_list<T> operator-(very_smart_list<T> const& lhs, very_smart_list<T> const& Rhs) {
		if (lhs.list.size() == 0 || Rhs.list.size() == 0)
			throw("Cannot subtract smart lists where one is empty");
		
		very_smart_list<T> result(0);
		std::vector<bool> used(Rhs.list.size(), false);

		for (const auto& item : lhs.list) {
			auto it = std::find_if(Rhs.list.begin(), Rhs.list.end(),
								[&used, &Rhs, &item](const T& val) {
									return val == item && !used[&val - &Rhs.list[0]];
								});

			if (it == Rhs.list.end()) {
				result.list.push_back(item);
			} else {
				used[it - Rhs.list.begin()] = true;
			}
		}

		return result;
	}
	template <typename T>
	very_smart_list<very_smart_list<T>> operator*(very_smart_list<T> const& lhs, very_smart_list<T> const& rhs) {
		std::size_t n = rhs.size();
		very_smart_list<T> ret(n);
		for (int i = 0; i < n; ++i) {
			very_smart_list<T> tmp = smart_list(n);
			for (auto j : rhs) {
				tmp.push_back(j*lhs[i]);
			}
			ret.push_back(tmp);
		}
		return ret;
	}
	template <typename T>
	std::ostream& operator<<(std::ostream& os, very_smart_list<T> const& sl) {
		for (auto i : sl.list) {
			os << "|" << i;
		}
		os << "|";
		return os;
	}
	template <typename T>
	bool operator==(very_smart_list<T> const& lhs, very_smart_list<T> const& rhs) {
		if (lhs.list.size() != rhs.list.size()) return false;
		int n = lhs.list.size();
		for (int i = 0; i < n; ++i) {
			if (lhs.list[i] != rhs.list[i]) return false;
		}
		return true;
	}
	template <typename T>
	very_smart_list<T> operator*(very_smart_list<T> const& lhs, double scalar) {
		
		std::vector<T> tmp;
		for (auto i : lhs.list) {
			i *= scalar;
			tmp.push_back(i);
		}
		very_smart_list<T> ret(tmp.size());
		ret.list = tmp;
		return ret;
	}
	template <typename T>
	very_smart_list<T> operator/(very_smart_list<T> const& lhs, double scalar) {
		very_smart_list<T> ret(0);
		for (auto i : lhs.list) {
			i /= scalar;
			ret.push_back(i);
		}
		//lhs = ret;
		return ret;
	}
} // namespace q1

#endif
