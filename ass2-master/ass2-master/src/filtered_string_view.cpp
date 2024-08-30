#include "./filtered_string_view.h"

namespace fsv {
	auto filtered_string_view::default_predicate(const char&) noexcept -> bool {
		return true;
	}
	filtered_string_view::iter::iter(char* ptr, filter pred) noexcept
	: _curr(ptr)
	, _predicate(pred) {}

	auto filtered_string_view::iter::operator*() const noexcept -> const char& {
		return *_curr;
	}
	auto filtered_string_view::iter::operator->() const noexcept -> const char* {
		return _curr;
	}
	auto filtered_string_view::iter::operator++() noexcept -> iterator& {
		do {
			++_curr;
		} while (*_curr != '\0' && !_predicate(*_curr));
		return *this;
	}
	auto filtered_string_view::iter::operator++(int) noexcept -> iterator {
		iter tmp = *this;
		while (*_curr != '\0' && !_predicate(*_curr)) {
			++_curr;
		}
		return tmp;
	}
	auto filtered_string_view::iter::operator--() noexcept -> iterator& {
		do {
			--_curr;
		} while (!_predicate(*_curr));
		return *this;
	}
	auto filtered_string_view::iter::operator--(int) noexcept -> iterator {
		iter tmp = *this;
		while (!_predicate(*_curr)) {
			--_curr;
		}
		return tmp;
	}

	auto operator==(const filtered_string_view::iter& left, const filtered_string_view::iter& right) noexcept -> bool {
		return left._curr == right._curr;
	}
	auto operator!=(const filtered_string_view::iter& left, const filtered_string_view::iter& right) noexcept -> bool {
		return left._curr != right._curr;
	}
	filtered_string_view::filtered_string_view() noexcept
	: _str(nullptr)
	, _size(0)
	, _predicate(default_predicate) {}

	auto filtered_string_view::predicate_str(const char* str, filter predicate) noexcept -> std::shared_ptr<char[]> {
		std::size_t size = 0;
		for (const char* ptr = str; *ptr != '\0'; ++ptr) {
			if (predicate(*ptr)) {
				++size;
			}
		}
		std::shared_ptr<char[]> ret = std::shared_ptr<char[]>(new char[size + 1], std::default_delete<char[]>());
		char* ptr_ret = ret.get();
		for (const char* ptr = str; *ptr != '\0'; ++ptr) {
			if (predicate(*ptr)) {
				*ptr_ret++ = *ptr;
			}
		}
		*ptr_ret = '\0';
		return ret;
	}
	filtered_string_view::filtered_string_view(const char* str) noexcept {
		_size = std::strlen(str);
		_str = std::shared_ptr<char[]>(new char[_size + 1], std::default_delete<char[]>());
		std::strcpy(_str.get(), str);
		_predicate = default_predicate;
	}
	filtered_string_view::filtered_string_view(const std::string& str) noexcept {
		_str = std::shared_ptr<char[]>(new char[str.size() + 1], std::default_delete<char[]>());
		std::strcpy(_str.get(), str.c_str());
		_predicate = default_predicate;
		_size = str.size();
	}
	filtered_string_view::filtered_string_view(const std::string& str, filter predicate) noexcept {
		_str = std::shared_ptr<char[]>(new char[str.size() + 1], std::default_delete<char[]>());
		std::strcpy(_str.get(), str.c_str());
		_predicate = predicate;
		_size = str.size();
	}
	filtered_string_view::filtered_string_view(const filtered_string_view& other) noexcept {
		_str = other._str;
		_size = other._size;
		_predicate = other._predicate;
	}
	filtered_string_view::filtered_string_view(filtered_string_view&& other) noexcept
	: _str(other._str)
	, _size(other._size)
	, _predicate(other._predicate) {
		other._str = nullptr;
		other._size = 0;
		other._predicate = default_predicate;
	}
	filtered_string_view::~filtered_string_view() noexcept {}
	auto filtered_string_view::begin() const noexcept -> iterator {
		return iter(const_cast<char*>(_str.get()), _predicate);
	}
	auto filtered_string_view::end() const noexcept -> iterator {
		return iter(const_cast<char*>(_str.get()) + _size, _predicate);
	}
	auto filtered_string_view::cbegin() const noexcept -> const_iterator {
		return iter(const_cast<char*>(_str.get()), _predicate);
	}
	auto filtered_string_view::cend() const noexcept -> const_iterator {
		return iter(const_cast<char*>(_str.get()) + _size, _predicate);
	}
	auto filtered_string_view::rbegin() noexcept -> std::reverse_iterator<iterator> {
		return std::reverse_iterator(this->end());
	}
	auto filtered_string_view::rend() noexcept -> std::reverse_iterator<iterator> {
		return std::reverse_iterator(this->begin());
	}
	auto filtered_string_view::crbegin() const noexcept -> std::reverse_iterator<const_iterator> {
		return std::reverse_iterator(this->cend());
	}
	auto filtered_string_view::crend() const noexcept -> std::reverse_iterator<const_iterator> {
		return std::reverse_iterator(this->cbegin());
	}
	auto filtered_string_view::size() const noexcept -> std::size_t {
		if (this->_str == nullptr)
			return 0;
		std::shared_ptr<char[]> predicated_str = filtered_string_view::predicate_str(this->data(), this->predicate());

		std::size_t ret = std::strlen(predicated_str.get());
		return ret;
	}
	auto operator<<(std::ostream& os, const filtered_string_view& fsv) -> std::ostream& {
		if (fsv.data() == nullptr)
			return os;
		std::shared_ptr<char[]> predicated_str = filtered_string_view::predicate_str(fsv.data(), fsv.predicate());
		os << predicated_str;
		return os;
	}
	auto operator<<(std::ostream& os, filtered_string_view::const_iterator& it) -> std::ostream& {
		return os << *it;
	}
	auto filtered_string_view::operator=(const filtered_string_view& str) noexcept -> filtered_string_view& {
		if (this != &str) {
			_str = str._str;
			_size = str._size;
			_predicate = str._predicate;
		}
		return *this;
	}
	auto filtered_string_view::operator=(filtered_string_view&& other) noexcept -> filtered_string_view& {
		if (this != &other) {
			this->_str = std::move(other._str);
			this->_size = other._size;
			this->_predicate = other._predicate;
			other._size = 0;
			other._predicate = default_predicate;
		}
		return *this;
	}
	auto operator==(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool {
		if (left.size() != right.size())
			return false;

		for (std::size_t i = 0; i < left.size(); ++i)
			if (left[i] != right[i])
				return false;
		return true;
	}
	auto operator<(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool {
		const size_t left_length = left.size();
		const size_t right_length = right.size();

		if (right_length == left_length) {
			for (std::size_t i = 0; i < left_length; ++i)
				if (left[i] != right[i])
					return left[i] < right[i];
			return false;
		}
		else {
			return left_length < right_length ? true : false;
		}
	}
	auto operator<=(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool {
		return left < right || left == right;
	}
	auto operator>(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool {
		const size_t left_length = left.size();
		const size_t right_length = right.size();

		if (right_length == left_length) {
			for (std::size_t i = 0; i < left_length; ++i) {
				if (left[i] != right[i])
					return left[i] > right[i];
			}
			return false;
		}
		else {
			return left_length > right_length ? true : false;
		}
	}
	auto operator>=(const filtered_string_view& left, const filtered_string_view& right) noexcept -> bool {
		return left > right || left == right;
	}
	auto operator<=>(const filtered_string_view& left, const filtered_string_view& right) noexcept
	    -> std::strong_ordering {
		if (left < right)
			return std::strong_ordering::less;
		if (left > right)
			return std::strong_ordering::greater;
		return std::strong_ordering::equal;
	}
	auto filtered_string_view::operator[](std::size_t index) const noexcept -> char {
		assert(index < _size);
		std::shared_ptr<char[]> predicated_str = filtered_string_view::predicate_str(this->data(), this->predicate());
		char ret = predicated_str.get()[index];
		return ret;
	}
	filtered_string_view::operator std::string() const noexcept {
		std::string ret;
		for (auto it = this->begin(); it != this->end(); ++it) {
			ret.push_back(*it);
		}
		return ret;
	}
	auto filtered_string_view::data() const noexcept -> const char* {
		return _str.get();
	}
	auto filtered_string_view::c_str() const noexcept -> const char* {
		return _str.get();
	}
	auto filtered_string_view::at(int index) -> const char& {
		if (static_cast<std::size_t>(index) >= this->size())
			throw std::domain_error{"filtered_string_view::at(" + std::to_string(index) + "): invalid index"};
		auto count = 0;
		auto predicate = this->predicate();

		for (const char* ptr = _str.get(); *ptr != '\0'; ++ptr) {
			if (predicate(*ptr)) {
				++count;
			}
			if (count - 1 == index)
				return *ptr;
		}
		throw std::domain_error{"filtered_string_view::at(" + std::to_string(index) + "): invalid index"};
	}
	auto filtered_string_view::at(std::size_t index) const -> const char& {
		if (index >= this->size())
			throw std::domain_error{"filtered_string_view::at(" + std::to_string(index) + "): invalid index"};
		std::size_t count = 0;
		auto predicate = this->predicate();

		for (const char* ptr = _str.get(); *ptr != '\0'; ++ptr) {
			if (predicate(*ptr)) {
				++count;
			}
			if (count - 1 == index)
				return *ptr;
		}
		throw std::domain_error{"filtered_string_view::at(" + std::to_string(index) + "): invalid index"};
	}
	auto filtered_string_view::empty() noexcept -> bool {
		return this->size() == 0 ? true : false;
	}
	auto filtered_string_view::predicate() const noexcept -> const filter& {
		return _predicate;
	}
	auto compose(const filtered_string_view& fsv, const std::vector<filter>& filts) noexcept -> filtered_string_view {
		auto origin_predicate = fsv.predicate();
		auto new_predicate = [filts, origin_predicate](const char& ch) {
			for (const auto& filt : filts) {
				if (!origin_predicate(ch) || !filt(ch)) {
					return false;
				}
			}
			return true;
		};
		return filtered_string_view{fsv.data(), new_predicate};
	}
	auto split(const filtered_string_view& fsv, const filtered_string_view& tok) noexcept
	    -> std::vector<filtered_string_view> {
		std::vector<filtered_string_view> ret;
		if (tok.size() == 0) {
			ret.push_back(fsv);
			return ret;
		}
		const char* delimiter = tok.data();
		const std::size_t delimiter_len = std::strlen(delimiter);
		std::shared_ptr<char[]> filtered_data = filtered_string_view::predicate_str(fsv.data(), fsv.predicate());

		char* start = filtered_data.get();
		char* pos = strstr(start, delimiter);

		while (pos != nullptr) {
			*pos = '\0';
			ret.push_back(filtered_string_view(start, fsv.predicate()));
			start = pos + delimiter_len;
			pos = strstr(start, delimiter);
		}

		if (start != nullptr || std::strlen(start) == 0) {
			ret.push_back(filtered_string_view(start, fsv.predicate()));
		}
		return ret;
	}
	auto substr(const filtered_string_view& fsv, int pos, int count) noexcept -> filtered_string_view {
		if (pos == 0 && count == 0)
			return filtered_string_view("", fsv.predicate());
		const std::size_t start_pos = static_cast<std::size_t>(pos);
		const std::size_t rcount = count <= 0 ? fsv.size() - start_pos : static_cast<std::size_t>(count);
		std::vector<filter> filts;
		auto origin_predicate = fsv.predicate();
		auto predicate1 = [start_pos, rcount, &fsv, origin_predicate](const char& ch) {
			for (std::size_t i = start_pos; i < rcount + start_pos; ++i) {
				if (origin_predicate(ch) && fsv.at(i) == ch)
					return true;
			}
			return false;
		};
		filts.push_back(predicate1);
		filtered_string_view ret = compose(fsv, filts);
		return ret;
	}
} // namespace fsv