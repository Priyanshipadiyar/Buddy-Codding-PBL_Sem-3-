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


class Queue {
    Stack s1; Stack s2;
public:
    void enqueue(int value) { s1.push(value); }
    int dequeue() {
        if (s2.isEmpty()) {
            if (!s1.isEmpty()) s2.push(s1.pop());
        }
        return s2.isEmpty() ? -1 : s2.pop();
    }
};

int main(void) {
    Queue queue;
    char command[16]; int value;
    while (scanf("%15s", command) == 1) {
        if (strcmp(command, "push") == 0) {
            if (scanf("%d", &value) != 1) break;
            queue.enqueue(value);
        } else if (strcmp(command, "pop") == 0) {
            printf("%d\n", queue.dequeue());
        }
    }
    return 0;
}
