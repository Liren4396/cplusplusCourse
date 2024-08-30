#ifndef RECORD_HPP
#define RECORD_HPP
#include <unordered_map>
#include <ostream>

namespace q2 {
	class record {
	public:


	public:
		record() {};
		~record() {};
		auto get_value(std::string const& s) const -> std::string {
			auto it = hash.find(s);
			if (it != hash.end()) {
				return it->second;
			}
			return "Undefined";
		}

		auto has_attribute(std::string const& key) const -> bool {
			return hash.find(key) != hash.end();
		}
		auto count() const -> std::size_t {
			return hash.size();
		}

		auto delete_attribute(std::string const& key) -> bool {
			for (auto [k, v] : hash) {
				if (key == k) {
					hash.erase(k);
					return true;
				}
			}
			return false;
		}
		auto set_value(std::string const& key, std::string const& val) -> void {
			for (auto [k, v] : hash) {
				if (k == key) return;
			}
			hash[key] = val;
		}
		friend auto operator<<(std::ostream& os, record const& g) -> std::ostream& {
			os << "{\n";
			for (const auto& [key, value] : g.hash) {
				os << key << "=" << value << "\n";
			}
			os << "}\n";
			return os;
		}
		friend auto operator>>(std::istream& is, record& sm) -> std::istream& {
			std::string line;
			
			while (true) {
				std::getline(is, line);  // Read each subsequent line
				if (line == "}") break;  // If the line is the closing brace, stop reading
				auto delimiter_pos = line.find('=');
				if (delimiter_pos != std::string::npos) {
					auto key = line.substr(0, delimiter_pos);
					auto value = line.substr(delimiter_pos + 1);
					sm.set_value(key, value);
				}
			}	
			return is;
		}
	private:
		std::unordered_map<std::string, std::string> hash;
	};
} // namespace q2

#endif // RECORD_HPP

