#ifndef DATABASE_HPP
#define DATABASE_HPP
#include <vector>
#include <istream>
#include "record.h"
#include "query.h"
namespace q2 {
	class record;
	class query;
	class database {
	public:
		database() = default;
		~database() = default;

		auto insert(record const& r) -> void {
			records_.emplace_back(r);
		}

		auto count() const -> std::size_t {
			return records_.size();
		}

		friend auto operator<<(std::ostream& os, database const& g) -> std::ostream& {
			for (const auto& rec : g.records_) {
				os << rec;
			}
			return os;
		}
		friend auto operator>>(std::istream& is, database& sm) -> std::istream& {
			record rec;
			while (is >> rec) {
				sm.insert(rec);
			}
			return is;
		}
		auto delete_matching(query const& q) -> std::size_t {
			auto original_size = records_.size();
			for (auto it = records_.begin(); it != records_.end(); ++it) {
				if (q.matches(*it)) records_.erase(it);
			}
			return original_size - records_.size();
		}
		auto select(query const& q) const -> database {
			database n;
			for (const auto& rec : records_) {
				if (q.matches(rec)) {
					n.insert(rec);
				}
			}
			return n;
		}
	private:
		std::vector<record> records_;
	};

} // namespace q2

#endif // DATABASE_HPP
