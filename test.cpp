#include <iostream>
#include <string>
#include <vector>
#include <string_view>
#define N 3
using namespace std;

vector<int> searchRange(vector<int>& nums, int target) {
    if (nums.size() == 0) return {-1, -1};
    int left = 0, right = nums.size()-1;
    while (left <= right) {
        int mid = left + (right-left) / 2;
        if (nums[mid] < target) {
            left = mid+1;
        } else if (nums[mid] > target) {
            right = mid+1;
        } else {
            int l = mid, r = mid;
            while (l > 0 && nums[l-1] == nums[l]) --l;
            while (r < nums.size()-1 && nums[r+1] == nums[r]) ++r;
            return {nums[l], nums[r]};
        }
    }
    return {-1, -1};
}
namespace my_namespace {
    class my_class {
        public:
            my_class():
                _word("hello world"){};
            void print() {
                cout << _word << endl;
            }
        private:
            string _word;
    };
};
/*
int main() {
    my_namespace::my_class tmp;
    tmp.print();
    return 0;
}*/

#include <thread>
#include <mutex>

int count = 0;

void increment_counter(int times) {
    static mutex mymutex;
    for (int i = 0; i < times; ++i) {
        mymutex.lock();
        ++count;
        mymutex.unlock();
    }
}
std::timed_mutex mtx;

void time_lock_function() {
    auto start = std::chrono::high_resolution_clock::now();
    if (mtx.try_lock_for(std::chrono::seconds(2))) {
        std::cout << "Thread " << std::this_thread::get_id() << " locked mutex.\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        mtx.unlock();
        std::cout << "Thread " << std::this_thread::get_id() << " unlocked mutex.\n";
    } else {
        std::cout << "Thread " << std::this_thread::get_id() << " failed to lock mutex with\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Thread " << std::this_thread::get_id() << " took " << diff.count() << " seconds" << endl;
}
int main() {
    
    std::thread t1(time_lock_function);
    t1.join();
}