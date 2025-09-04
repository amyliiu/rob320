#include <iostream>
#include <stack>
#include <string>

using namespace std;

// Given a string s containing just the characters '(', ')', '{', '}', '['
// and ']', determine if the input string is valid.

// An input string is valid if:
//   * Open brackets must be closed by the same type of brackets.
//   * Open brackets must be closed in the correct order.
//   * Every close bracket has a corresponding open bracket of the same type.

// Examples:
// 1. Input: s = "()"
//    Output: true
// 2. Input: s = "()[]{}"
//    Output: true
// 3. Input: s = "(]"
//    Output: false
// 4. Input: s = "([)]"
//    Output: false

bool is_valid(std::string s) {
    // TODO: Implement the function
    stack<char> st;
    for(int i = 0; i < s.size(); i++) {
        if(s[i] == '(' || s[i] == '{' || s[i] == '['){
            st.push(s[i]);
        }
        else{
            char next = st.top();
            if(( s[i] == ')' && next != '(') ||
               ( s[i] == '}' && next != '{') ||
               ( s[i] == ']' && next != '[')){
                return false;
               }
            st.pop();
        }
    }

    return st.size() == 0;
}

int main(int argc, char* argv[]) {
    /* Try passing in:
    *   - "()"
    *   - "()[]{}"
    *   - "(]"
    *   - "([)]"
    *   - "{[]}"
    */

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <string>" << std::endl;
        return 1;
    }
    std::string s = argv[1];
    std::cout << is_valid(s) << std::endl;
    return 0;
}
