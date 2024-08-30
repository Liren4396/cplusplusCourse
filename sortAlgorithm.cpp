#include <iostream>
#include <vector>
#include <climits>
using namespace std;

void InsertSort(vector<int>& nums) {
    for (int i = 0; i < nums.size()-1; ++i) {
        int end = i;
        int x = nums[end+1];
        while (end >= 0 && x < nums[end]) {
            nums[end+1] = nums[end];
            end--;
        }
        nums[end+1] = x;
    }
}
void shellSort(vector<int>& nums) {
    int gap = nums.size();
    int n = nums.size();
    while (gap > 1) {
        gap = gap/2;
        //gap /= 2;
        for (int i = 0; i < n-gap; ++i) {
            int end = i;
            int x = nums[end+gap];
            while (end >= 0) {
                if (nums[end] > x) {
                    nums[end+gap] = nums[end];
                    end -= gap;
                } else {
                    break;
                }
            }
            nums[end+gap] = x;
        }
    }
}
void print_nums(const vector<int>& nums) {
    for (auto x : nums) {
        cout << x << " ";
    }
    cout << endl;
}
void selectSort(vector<int>& nums) {
    int begin = 0;
    int end = nums.size()-1;
    while (begin < end) {
        int min_val = begin;
        int max_val = begin;
        for (int i = begin; i <= end; ++i) {
            if (nums[min_val] > nums[i]) {
                min_val = i;
            }
            if (nums[max_val] < nums[i]) {
                max_val = i;
            }
        }
        swap(nums[begin], nums[min_val]);
        //if (max_val == begin) max_val = min_val;
        print_nums(nums);
        swap(nums[end], nums[max_val]);
        print_nums(nums);
        begin++;
        end--;
    }
}
void AdjustDown(vector<int>& nums, int n, int pos) {
    int parent = pos;
    int child =  parent*2+1;
    while (child < n) {
        if (child+1 < n && nums[child+1] > nums[child]) child++;
        else if (nums[child] > nums[parent]) {
            swap(nums[parent], nums[child]);
            parent = child;
            child = parent*2+1;
        } else break;
    }
}

void heapSort(vector<int>& nums) {
    int n = nums.size();
    for (int i = (n-1-1)/2; i >= 0; i--) {
        AdjustDown(nums, n, i);
        print_nums(nums);
    }
    
    for (int i = n-1; i >= 0; i--) {
        swap(nums[i], nums[0]);
        AdjustDown(nums, i, 0);
        print_nums(nums);
    }
}

void bubbleSort(vector<int>& nums) {
    int n = nums.size();
    int flag = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (nums[j] > nums[j+1]) {
                swap(nums[j+1], nums[j]);
                flag = 1;
            }
        }
        if (flag == 0) break;
    }
}

int PartSort(vector<int>& nums, int left, int right) {
    int key = right;
    while (left < right) {
        while (left < right && nums[left] <= nums[key]) left++;
        while (left < right && nums[right] >= nums[key]) right--;
        swap(nums[left], nums[right]);
    }
    swap(nums[left], nums[key]);
    return left;
}

int PartSort1(vector<int>& nums, int left, int right) {
    int key = nums[left];
    int hole = left;
    while (left < right) {
        while (left < right && key <= nums[right]) right--;
        nums[hole] = nums[right];
        hole = right;
        while (left < right && nums[left] <= nums[key]) left++;
        nums[hole] = nums[left];
        hole = left;
    }
    nums[hole] = key;
    return left;
}
int PartSort2(vector<int>& nums, int left, int right) {
    int keyi = left;
    int prev = left;
    int cur = left + 1;
    while (cur <= right) {
        if (nums[cur] < nums[keyi] && ++prev != cur) swap(nums[prev], nums[cur]);
        cur++;
    }
    swap(nums[prev], nums[keyi]);
    return prev;
}
void quickSort(vector<int>& nums, int left, int right) {
    if (left >= right) return;
    int keyi = PartSort2(nums, left, right);
    quickSort(nums, left, keyi-1);
    quickSort(nums, keyi+1, right);
}

void merge(vector<int>& nums, int left, int mid, int right) {
    vector<int> leftSub(nums.begin()+left, nums.begin()+mid+1);
    vector<int> rightSub(nums.begin()+mid+1, nums.begin()+right+1);
    int leftPos = 0, rightPos = 0, mergedPos = left;
    while (leftPos < leftSub.size() && rightPos < rightSub.size()) {
        if (leftSub[leftPos] <= rightSub[rightPos]) {
            nums[mergedPos] = leftSub[leftPos];
            leftPos++;
        } else {
            nums[mergedPos] = rightSub[rightPos];
            rightPos++;
        }
        mergedPos++;
    }
    while (leftPos < leftSub.size()) {
        nums[mergedPos] = leftSub[leftPos];
        leftPos++;
        mergedPos++;
    }
    while (rightPos < rightSub.size()) {
        nums[mergedPos] = rightSub[rightPos];
        rightPos++;
        mergedPos++;
    }
}
void mergeSort(vector<int>& nums, int left, int right) {
    if (left >= right) return;
    int mid = (left+right) / 2;
    mergeSort(nums, left, mid);
    mergeSort(nums, mid+1, right);
    merge(nums, left, mid, right);
}


void countSort(vector<int>& nums) {
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    for (auto num : nums) {
        min_val = min(min_val, num);
        max_val = max(max_val, num);
    }
    vector<int> tmp(max_val, 0);
    for (auto num : nums) {
        tmp[num]++;
    }
    int j = 0;
    for (int i = min_val; i <= max_val; i++) {
        while (tmp[i] != 0) {
            nums[j++] = i;
            tmp[i]--;
        }
    }
}
/*
void CheckSystem() {
    int a = 1;
    cout << hex << a << endl;
    int num = (*(char*)&a);
    if (num == 1) cout << "小端" << endl;
    else cout << "大端" << endl;
}
int main() {
    CheckSystem();
}*/
#include <atomic>

int main() {
    atomic<int> atomicInt(0);
    atomicInt.store(10);
    int value = atomicInt.load();
    cout << "Value: " << value << endl;

    int exchangedValue = atomicInt.exchange(20);
    cout << "exchanged val: (also 10) " << exchangedValue << endl;
    cout << "new val: (also 20) " << atomicInt.load() << endl;
}
/*
int main() {
    vector<int> nums = {1,3,2,5,4,7};
    // InsertSort(nums);
    // shellSort(nums);
    // selectSort(nums);
    // heapSort(nums);
    // bubbleSort(nums);
    // quickSort(nums, 0, nums.size()-1);
    // mergeSort(nums, 0, nums.size()-1);
    countSort(nums);
    for (auto x : nums) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}*/