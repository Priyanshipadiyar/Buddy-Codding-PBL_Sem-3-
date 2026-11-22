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

int applyOperator(int first, int second, char op) {
    if (op == '+') return first + second;
    if (op == '-') return first - second;
    if (op == '*') return first * second;
    return first / second;
}
int evaluatePostfix(char expression[]) {
    Stack stack;
    char* token = strtok(expression, " \r\n\t");
    while (token) {
        if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            int second = stack.peek();
            int first = stack.pop();
            stack.push(applyOperator(first, second, token[0]));
        } else { stack.push(atoi(token)); }
        token = strtok(NULL, " \r\n\t");
    }
    return stack.pop();
}
int main(void) {
    char expression[10000]; if (!fgets(expression, sizeof(expression), stdin)) return 0;
    printf("%d\n", evaluatePostfix(expression)); return 0;
}
