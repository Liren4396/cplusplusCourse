#ifndef QUERY_HPP
#define QUERY_HPP
#include <string>
#include "record.h"
namespace q2 {
	class record;

	class query {
	public:
		virtual ~query() = default;
		[[nodiscard]] auto virtual matches(record const&) const -> bool = 0;
		[[nodiscard]] virtual auto clone() const -> query* = 0;
	};

	class query_equals : public query {
	public:
		query_equals(std::string attr, std::string value)
		: attr_(std::move(attr)),
		  value_(std::move(value)) {}
		auto matches(const record& r) const -> bool override {
			return r.has_attribute(attr_) && r.get_value(attr_) == value_;
		}
		auto clone() const -> query* override {
			return new query_equals(*this);
		}
	private:
		std::string attr_;
		std::string value_;
	};
	class query_less_than : public query {
	public:
        query_less_than(std::string attr, std::string value)
            : attr_(std::move(attr)), value_(std::move(value)) {}

        auto matches(const record& r) const -> bool override {
            return r.has_attribute(attr_) && r.get_value(attr_) < value_;
        }

        auto clone() const -> query* override {
            return new query_less_than(*this);
        }

    private:
        std::string attr_;
        std::string value_;
	};
	
	class query_greater_than : public query {
	public:
        query_greater_than(std::string attr, std::string value)
		: attr_(std::move(attr)),
		  value_(std::move(value)) {}

        auto matches(const record& r) const -> bool override {
            return r.has_attribute(attr_) && r.get_value(attr_) > value_;
        }

        auto clone() const -> query* override {
            return new query_greater_than(*this);
        }

    private:
        std::string attr_;
        std::string value_;
	};
	
	class query_starts_with : public query {
	public:
        query_starts_with(std::string attr, std::string value)
            : attr_(std::move(attr)), value_(std::move(value)) {}

        auto matches(const record& r) const -> bool override {
            return r.has_attribute(attr_) && r.get_value(attr_) == "0";
        }

        auto clone() const -> query* override {
            return new query_starts_with(*this);
        }

    private:
        std::string attr_;
        std::string value_;
	};
	
	class query_and  : public query {
	public:
        query_and(query* q1, query* q2)
            : q1_(q1), q2_(q2) {}
		~query_and() {
            delete q1_;
            delete q2_;
        }
        auto matches(const record& r) const -> bool override {
            return q1_->matches(r) && q2_->matches(r);
        }

        auto clone() const -> query* override {
            return new query_and(q1_->clone(), q2_->clone());
        }

    private:
        query* q1_;
        query* q2_;
	};
	class query_or : public query {
	public:
        query_or(query* q1, query* q2)
            : q1_(q1), q2_(q2) {}
		~query_or() {
            delete q1_;
            delete q2_;
        }
        auto matches(const record& r) const -> bool override {
            return q1_->matches(r) || q2_->matches(r);
        }

        auto clone() const -> query* override {
            return new query_or(q1_->clone(), q2_->clone());
        }

    private:
        query* q1_;
        query* q2_;
	};
	class query_not : public query {
	public:
        query_not(query* q)
            : q_(q) {}
		~query_not() {
            delete q_;
        }
        auto matches(const record& r) const -> bool override {
            return !q_->matches(r);
        }

        auto clone() const -> query* override {
            return new query_not(q_->clone());
        }

    private:
        query* q_;
	};
} // namespace q2

#endif // QUERY_HPP
