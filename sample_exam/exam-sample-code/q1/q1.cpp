#include <iostream>
#include <sstream>
#include <stack>
#include <cmath>
#include <iomanip>
#include <vector>
#include <string>

namespace q1 {
    std::vector<std::string> output;
    std::stack<double> calc_stack;
    std::stack<bool> is_integer_stack; // 用于跟踪原始输入是否是整数
	std::vector<std::vector<std::string>> repeat_buffer;
	bool in_repeat = false;
	int repeat_count = 0;
    std::string format_output(double result, bool is_integer) {
        if (is_integer && result == static_cast<int>(result)) {
            // 如果原始输入是整数且结果也是整数，则只输出整数部分
            return std::to_string(static_cast<int>(result));
        } else {
            // 否则，输出三位小数
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(3) << result;
            return oss.str();
        }
    }
	std::vector<std::string> split_word(std::string const& command) {
		std::vector<std::string> ret;
		int j = 0;
		int i = 0;
		for (; i < command.size(); ++i) {
			if (command[i] == ' ') {
				ret.push_back(command.substr(j, i-j));
				j = i+1;
			}
		}
		if (i == command.size() && command[i-1] != ' ') {
			ret.push_back(command.substr(j, i-j));
		}
		return ret;
	}
    void process_line(const std::vector<std::string>& command) {
        for (auto word : command) {
			if (word == " ") continue;
			if (word == "add") {
				double num1 = calc_stack.top(); calc_stack.pop();
				bool is_integer1 = is_integer_stack.top(); is_integer_stack.pop();
				double num2 = calc_stack.top(); calc_stack.pop();
				bool is_integer2 = is_integer_stack.top(); is_integer_stack.pop();
				double ret = num1 + num2;
				calc_stack.push(ret);
				is_integer_stack.push(is_integer1 && is_integer2 && ret == static_cast<int>(ret));
				output.push_back(format_output(num1, is_integer1) + " + " + format_output(num2, is_integer2) + " = " + format_output(ret, is_integer_stack.top()));
			} else if (word == "sub") {
				double num1 = calc_stack.top(); calc_stack.pop();
				bool is_integer1 = is_integer_stack.top(); is_integer_stack.pop();
				double num2 = calc_stack.top(); calc_stack.pop();
				bool is_integer2 = is_integer_stack.top(); is_integer_stack.pop();
				double ret = num1 - num2;
				calc_stack.push(ret);
				is_integer_stack.push(is_integer1 && is_integer2 && ret == static_cast<int>(ret));
				output.push_back(format_output(num1, is_integer1) + " - " + format_output(num2, is_integer2) + " = " + format_output(ret, is_integer_stack.top()));
			} else if (word == "mult") {
				double num1 = calc_stack.top(); calc_stack.pop();
				bool is_integer1 = is_integer_stack.top(); is_integer_stack.pop();
				double num2 = calc_stack.top(); calc_stack.pop();
				bool is_integer2 = is_integer_stack.top(); is_integer_stack.pop();
				double ret = num1 * num2;
				calc_stack.push(ret);
				is_integer_stack.push(is_integer1 && is_integer2 && ret == static_cast<int>(ret));
				output.push_back(format_output(num1, is_integer1) + " * " + format_output(num2, is_integer2) + " = " + format_output(ret, is_integer_stack.top()));
			} else if (word == "div") {
				double num1 = calc_stack.top(); calc_stack.pop();
				bool is_integer1 = is_integer_stack.top(); is_integer_stack.pop();
				double num2 = calc_stack.top(); calc_stack.pop();
				bool is_integer2 = is_integer_stack.top(); is_integer_stack.pop();
				double ret = num2 / num1;
				calc_stack.push(ret);
				is_integer_stack.push(is_integer1 && is_integer2 && ret == static_cast<int>(ret));
				output.push_back(format_output(num2, is_integer2) + " / " + format_output(num1, is_integer1) + " = " + format_output(ret, is_integer_stack.top()));
			} else if (word == "sqrt") {
				double num = calc_stack.top(); calc_stack.pop();
				bool is_integer = is_integer_stack.top(); is_integer_stack.pop();
				double ret = std::sqrt(num); 
				calc_stack.push(ret);
				is_integer_stack.push(false); // sqrt 结果通常为浮点数
				output.push_back("sqrt " + format_output(num, is_integer) + " = " + format_output(ret, false));
			} else if (word == "pop") {
				if (calc_stack.empty()) return;
				calc_stack.pop();
				is_integer_stack.pop();
			} else if (word == "reverse") {
				int n = static_cast<int>(calc_stack.top());
				calc_stack.pop();
				is_integer_stack.pop();
				std::vector<double> temp_stack;
				std::vector<double> temp_is_integer_stack;
				for (int i = 0; i < n; ++i) {
					temp_stack.push_back(calc_stack.top());
					calc_stack.pop();
					temp_is_integer_stack.push_back(is_integer_stack.top());
					is_integer_stack.pop();
				}
				for (auto it = temp_stack.rbegin(); it != temp_stack.rend(); ++it) {
					calc_stack.push(*it);
				}
				for (auto it = temp_is_integer_stack.rbegin(); it != temp_is_integer_stack.rend(); ++it) {
					is_integer_stack.push(*it);
				}
			} else if (word == "repeat") {
				repeat_count = static_cast<int>(calc_stack.top());
				calc_stack.pop();
				is_integer_stack.pop();
				in_repeat = true;
			} else if (word == "endrepeat") {
				in_repeat = false;
				for (int i = 0; i < repeat_count; ++i) {
					for (const auto& cmd : repeat_buffer) {
						process_line(cmd);
					}
				}
				repeat_buffer.clear();
			}
			else {
				
				try {
					if (word.find('.') != std::string::npos) {
                        calc_stack.push(std::stod(word));
                        is_integer_stack.push(false); // 包含小数点的数为浮点数
                    } else {
                        calc_stack.push(static_cast<double>(std::stoi(word)));
                        is_integer_stack.push(true); // 没有小数点的数为整数
                    }
				} catch (std::invalid_argument& e) {
					;
				}
			}
		}
    }

    auto run_calculator(std::vector<std::string> const& input) -> std::vector<std::string> const {
		
		for (auto line : input) {
			if (in_repeat && line != "endrepeat") {
				repeat_buffer.push_back(split_word(line));
			} else {
				process_line(split_word(line));
			}
			
		}
		return output;
	}
}

int main() {
	auto const input = std::vector<std::string>{
		"10 20 sub",
		"4 3 add ",
		"2 mult",
		"16.0 ",
		"2 repeat",
		"sqrt",
		"endrepeat",
		"pop",
		"2.0",
		"mult",
		"3 repeat",
		"2 ",
		"2 reverse",
		"div",
		"3 mult",
		"endrepeat"
	};
	auto output = q1::run_calculator(input);
	for (auto str : output) {
		std::cout << str << std::endl;
	}
}
