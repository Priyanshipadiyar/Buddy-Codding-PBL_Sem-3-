#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>
#include <algorithm>
using std::scanf;
using std::printf;
using std::fgets;
using std::strtok;
using std::atoi;
using std::strcmp;

class Stack {
    int items[10000]{};
    int top = 0;
public:
    bool isEmpty() const { return top == 0; }
    void push(int value) { items[top++] = value; }
    int pop() { return items[--top]; }
    int peek() const { return items[top - 1]; }
};

int matches(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}
int isValid(const char text[]) {
    Stack stack;
    for (int i = 0; text[i]; ++i) {
        char ch = text[i];
        if (ch == '(' || ch == '[' || ch == '{') { stack.push(ch); }
        else {
            if (stack.isEmpty()) return 0;
            char opening = (char)stack.pop();
            if (!matches(opening, ch)) return 0;
        }
    }
    return 1;
}
int main(void) {
    char text[10000]; if (scanf("%9999s", text) != 1) return 0;
    printf("%s\n", isValid(text) ? "yes" : "no"); return 0;
}
