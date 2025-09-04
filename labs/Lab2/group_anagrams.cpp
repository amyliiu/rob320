#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;
// Given a list of strings, group anagrams together.
// An Anagram is a word or phrase formed by rearranging the letters of a
// different word or phrase, typically using all the original letters exactly
// once.

// Example:
// 1. Input: ["eat", "tea", "tan", "ate", "nat", "bat"]
//    Output: [["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]

std::vector<std::vector<std::string>> group_anagrams(std::vector<std::string>& strs) {
    // TODO: Implement the function
    std::vector<std::vector<std::string>> result;
    map<string, vector<string>> hashmap;
    for(string s : strs){
        string key = s;
        sort(key.begin(), key.end());
        hashmap[key].push_back(s);
    }

    for (auto& entry : hashmap) {
        result.push_back(entry.second);
    }
    
    return result;
}

int main() {
    std::vector<std::string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
    std::vector<std::vector<std::string>> result = group_anagrams(strs);
    for (std::vector<std::string> group : result) {
        for (std::string s : group) {
            std::cout << s << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}