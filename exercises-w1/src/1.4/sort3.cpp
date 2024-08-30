#include "sort3.h"
#include <algorithm>
#include <vector>
auto sort3(int& a, int& b, int& c) -> void
{
    std::vector<int> list = {a, b, c};
    sort(list.begin(), list.end());
    a = list[0];
    b = list[1];
    c = list[2];
}

auto sort3(std::string& a, std::string& b, std::string& c) -> void
{
    std::vector<std::string> list = {a, b, c};
    sort(list.begin(), list.end());
    a = list[0];
    b = list[1];
    c = list[2];
}