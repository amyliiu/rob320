#include <queue>
#include <vector>
#include <iostream>

using namespace std;

// Given an array of integers score, return an array of strings answer where answer[i] is one of the following:
//   * "Gold" if score[i] is the highest in the list.
//   * "Silver" if score[i] is the second highest in the list.
//   * "Bronze" if score[i] is the third highest in the list.

// Example:
// 1. Input: score = [5, 4, 3, 2, 1]
//    Output: ["Gold", "Silver", "Bronze", "4", "5"]
// 2. Input: score = [10, 3, 8, 9, 4]
//    Output: ["Gold", "5", "Bronze", "Silver", "4"]

class RelativeRanks {
public:
    std::vector<std::string> findRelativeRanks(std::vector<int>& score) {
        // TODO: Implement the function
        priority_queue<pair<int, int>> pq;

        for(int i = 0; i < score.size(); i++) {
            pq.push({score[i], i});
        }

        std::vector<std::string> result(score.size());

        int count = 1;
        while (pq.size() != 0){
            auto [val, i] = pq.top();
            pq.pop();

            if (count == 1){
                result[i] = "Gold";
            }
            else if(count == 2){
                result[i] ="Silver";
            }
            else if(count ==3){
                result[i] = "Bronze";
            }
            else{
                result[i] = to_string(count);
            }
            count++;
        }

        return result;
    }
};

int main() {
    RelativeRanks rr;
    std::vector<int> score = {5, 4, 3, 2, 1};
    std::vector<std::string> result = rr.findRelativeRanks(score);
    for (std::string s : result) {
        std::cout << s << " ";
    }
    std::cout << std::endl;

    score = {10, 3, 8, 9, 4};
    result = rr.findRelativeRanks(score);
    for (std::string s : result) {
        std::cout << s << " ";
    }
    std::cout << std::endl;
    return 0;
}