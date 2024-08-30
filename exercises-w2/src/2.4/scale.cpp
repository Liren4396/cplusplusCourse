#include "scale.h"
#include <vector>
#include <iostream>
auto scale(std::vector<int>& ivec, double factor) -> std::vector<double>
{
    std::vector<double> ret;
    for (int& i : ivec) {
        ret.emplace_back(i*factor);
    }

    return ret;
}
