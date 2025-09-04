#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// Given an array of integers nums and an integer target, return indices of the
// two numbers such that they add up to target.
// You may assume that each input would have exactly one solution, and you may
// not use the same element twice.
// You can return the answer in any order.

// Example:
// 1. Input: nums = [2,7,11,15], target = 9
//    Output: [0,1]
// 2. Input: nums = [3,2,4], target = 6
//    Output: [1,2]

std::pair<int, int> two_sum(std::vector<int>& nums, int target) {
    // TODO: Implement the function
    unordered_map<int, int> hashmap;

    for(int i=0; i < nums.size(); i++){
        int complement = target - nums[i];
        if (hashmap.find(complement) != hashmap.end()){
            return {hashmap[complement], i};
        }
        hashmap[nums[i]] = i;
    }
    return {};
}


int main() {
    std::vector<int> nums = {2, 7, 11, 15};
    int target = 9;
    std::pair<int, int> result = two_sum(nums, target);
    std::cout << result.first << " " << result.second << std::endl;

    nums = {3, 2, 4};
    target = 6;
    result = two_sum(nums, target);
    std::cout << result.first << " " << result.second << std::endl;
    return 0;
}