#include "word_ladder.h"

auto word_ladder::read_lexicon(const std::string &path) -> std::unordered_set<std::string> {
	std::ifstream in_file(path, std::ios::in);
	if (!in_file.is_open()) {
		std::cout << "Cannot Open File: " << path << std::endl;
		getchar();
		return {};
	}
	std::string line;
	std::unordered_set<std::string> ret;
	while(getline(in_file, line)) {
		ret.insert(line);
	}		
	return ret;
}

auto word_ladder::less_comparator(
    const std::vector<std::string> v1, 
    const std::vector<std::string> v2
) -> bool {
	return v1 < v2;
}

auto word_ladder::filtered_dict(
    std::size_t length, 
    const std::unordered_set<std::string> &lexicon
) -> std::unordered_set<std::string_view> {
	std::unordered_set<std::string_view> dict;
    for (const auto &word : lexicon) {
        if (word.size() == length) {
            dict.insert(std::string_view {word});
        }
    }
    return dict;
}

void word_ladder::swap_set_with_smaller_len(
    std::unordered_set<std::string> &begin_set, 
    std::unordered_set<std::string> &end_set, 
    bool &reversed, 
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &end_map
) {
	if (begin_set.size() > end_set.size()) {
        std::swap(begin_set, end_set);
        std::swap(begin_map, end_map);
        reversed = !reversed;
    }
}

void word_ladder::erase_word(
    std::unordered_set<std::string_view> &dict, 
    const std::unordered_set<std::string> &begin_set
) {
	for (const auto &word : begin_set) {
        dict.erase(word);
    }
}

void word_ladder::not_find_end_word(
    std::unordered_set<std::string> &next_level, 
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map, 
    const std::string &cur_word,
    const std::string &word
) {
	next_level.insert(cur_word);
    for (auto path : begin_map[word]) {
        path.push_back(cur_word);
        begin_map[cur_word].push_back(path);
    }
}

void word_ladder::found_end_word(
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &end_map,
    const bool &reversed,
    const std::string &cur_word,
    const std::string &word,
    std::vector<std::vector<std::string>> &ret
) {
	for (auto &path1 : begin_map[word]) {
        for (auto &path2 : end_map[cur_word]) {
            if (reversed) {
                std::vector<std::string> combined_path = path2;
                combined_path.insert(combined_path.end(), path1.rbegin(), path1.rend());
                ret.push_back(combined_path);
            } else {
                std::vector<std::string> combined_path = path1;
                combined_path.insert(combined_path.end(), path2.rbegin(), path2.rend());
                ret.push_back(combined_path);
            }
        }
    }
}

void word_ladder::sub_a_z_letter(
    std::unordered_set<std::string> &end_set,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &end_map,
    bool reversed,
    bool &found,
    std::vector<std::vector<std::string>> &ret,
    std::string &cur_word,
    std::string word,
    std::size_t &i,
    std::unordered_set<std::string> &next_level,
    std::unordered_set<std::string_view> &dict
) {
	for (char ch = 'a'; ch <= 'z'; ++ch) {
        if (cur_word[i] == ch) continue;
        cur_word[i] = ch;
        if (end_set.count(cur_word)) {
            found = true;
            found_end_word(begin_map, end_map, reversed, cur_word, word, ret);
        } else if (dict.count(cur_word)) {
            not_find_end_word(next_level, begin_map, cur_word, word);
        }
    }
}

void word_ladder::two_end_bfs(
    std::unordered_set<std::string> &begin_set,
    std::unordered_set<std::string> &end_set,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &end_map,
    bool reversed,
    std::vector<std::vector<std::string>> &ret,
    std::unordered_set<std::string_view> dict,
    std::size_t length,
    bool found) {
	while (!begin_set.empty() && !end_set.empty() && !found) {
        swap_set_with_smaller_len(begin_set, end_set, reversed, begin_map, end_map);
        std::unordered_set<std::string> next_level;
        erase_word(dict, begin_set);
        for (const auto &word : begin_set) {
            std::string cur_word = word;
            for (std::size_t i = 0; i < length; ++i) {
                char original_char = cur_word[i];
                sub_a_z_letter(end_set, begin_map, end_map, reversed, found, ret, cur_word, word, i, next_level, dict);
                cur_word[i] = original_char;
            }
        }
        begin_set = next_level;
    }
}

auto word_ladder::generate(
	const std::string &from,
	const std::string &to,
	const std::unordered_set<std::string> &lexicon
) -> std::vector<std::vector<std::string>> {
	std::size_t length = from.size();
    std::unordered_set<std::string_view> dict = filtered_dict(length, lexicon);

    std::unordered_set<std::string> begin_set = {from}, end_set = {to};
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> begin_map, end_map;
    begin_map[from] = {{from}};
    end_map[to] = {{to}};
    bool found = false, reversed = false;
    std::vector<std::vector<std::string>> ret;
    two_end_bfs(begin_set, end_set, begin_map, end_map, reversed, ret, dict, length, found);
    std::sort(ret.begin(), ret.end(), less_comparator);
    return ret;
}