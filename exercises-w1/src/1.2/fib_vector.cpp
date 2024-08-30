#include "fib_vector.h"
#include <iostream>
#include <vector>
auto fibonacci(int n) -> std::vector<int>
{
    auto nums = std::vector<int>(n);
    
    if (n == 0) {
        return {};
    } else if (n == 1) {
        return {1};
    } else if (n == 2) {
        return {1, 2};
    }
    int sz = nums.size();
    nums[0] = 1;
    nums[1] = 2;
    for (int i = 2; i < sz; i++) {
        nums[i] = nums[i-1] + nums[i-2];
    }
    return nums;
}
