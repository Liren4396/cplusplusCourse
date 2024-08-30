#ifndef COMP6771_WORD_LADDER_H
#define COMP6771_WORD_LADDER_H

#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <set>
#include <climits>

/*
    Why I choose Two-End-BFS to solve this assignment:
    BFS is too slow that can't run the benchmark test in 12 seconds.
    Two-End-BFS improves traditional BFS that one point is started from start point
    And one point is started from end point.
    So it increases the performance a lot and increases the searching speed.
    To time complexity, two end bfs only search the half depth when traditional bfs needs
    to spent whole one depth.
*/

namespace word_ladder {
// Given a file path to a newline-separated list of words...
// Loads those words into an unordered set and returns it.
auto read_lexicon(const std::string& path) -> std::unordered_set<std::string>;

// Given a start word and destination word, returns all the shortest possible paths from the
// start word to the destination, where each word in an individual path is a valid word per the
// provided lexicon.
// Preconditions:
// - from.size() == to.size()
// - lexicon.contains(from)
// - lexicon.contains(to)
auto generate(
    const std::string& from,
    const std::string& to,
    const std::unordered_set<std::string>& lexicon) -> std::vector<std::vector<std::string>>;

// Compare two vectors with ascending order.
// Preconditions:
// - v1 and v2 are not empty.
auto less_comparator(
    const std::vector<std::string> v1, 
    const std::vector<std::string> v2) -> bool;

// Filter lexicon with length.
// Find the same length word in lexicon and add to a new dictionary with type of string_view.
// Preconditions:
// length >= 0.
// length <= INT_MAX.
auto filtered_dict(
    std::size_t length, 
    const std::unordered_set<std::string> &lexicon
) -> std::unordered_set<std::string_view>;

// Swap Set1 and Set2 when length of Set1 is larger than Set2, also swap Map1 and Map2.
// Use reversed as control flag that distinguish with Setss original status.
// Preconditions:
// begin_set, end_set are exist.
// begin_map, end_map are exist.
void swap_set_with_smaller_len(
    std::unordered_set<std::string> &begin_set, 
    std::unordered_set<std::string> &end_set, 
    bool &reversed, 
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &end_map
);

// Erase dict's word that is also in begin_set
void erase_word(
    std::unordered_set<std::string_view> &dict, 
    const std::unordered_set<std::string> &begin_set
);

// In a-z word sub loop, find a word that is in dict but is not the end word.
// We need to add current word to the loop level that represent increasing the loop level.
// And then and the current word to the map .
void not_find_end_word(
    std::unordered_set<std::string> &next_level, 
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map, 
    const std::string &cur_word,
    const std::string &word
);

// In a-z word sub loop, find a word that is end word.
// We need to extract word from begin_map and end_map by using cur_word and word
// To find its effecienct position.
// And then use reversed flag to combine 
// its order with { words in begin_map + the words in end_map };
// Push it to ret as one result and establish a loop to extract all results.
void found_end_word(
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &end_map,
    const bool &reversed,
    const std::string &cur_word,
    const std::string &word,
    std::vector<std::vector<std::string>> &ret
);

// Sub word with the pos by using loop of a-z to get a new word.
// Use the word compared to the word in end_set.
// If it's not in end_set but it occurs in dict
// We can add the new word to the next_level and begin_map
// If it's in end_set, we can find the shortest path by extracting
// the words from begin_set and end_set. 
void sub_a_z_letter(
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
);

// Use Two-End-BFS to improve the original BFS algorithm.
// Define two sets -- begin_set with from and end_set with to.
// Use begin_set and end_set to instead of one queue in loop.
// begin_map and end_map to record all possible words.
// Use begin_set as main queue to go loop when begin_set is lager than
// end_set.
void two_end_bfs(
    std::unordered_set<std::string> &begin_set,
    std::unordered_set<std::string> &end_set,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &begin_map,
    std::unordered_map<std::string, std::vector<std::vector<std::string>>> &end_map,
    bool reversed,
    std::vector<std::vector<std::string>> &ret,
    std::unordered_set<std::string_view> dict,
    std::size_t length,
    bool found
);
} // namespace word_ladder
#endif // COMP6771_WORD_LADDER_H
