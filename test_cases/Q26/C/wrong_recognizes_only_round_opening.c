#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

typedef struct Stack { int items[10000]; int top; } Stack;
void initStack(Stack* stack) { stack->top = 0; }
int isEmpty(const Stack* stack) { return stack->top == 0; }
void push(Stack* stack, int value) { stack->items[stack->top++] = value; }
int pop(Stack* stack) { return stack->items[--stack->top]; }
int peek(const Stack* stack) { return stack->items[stack->top - 1]; }

int matches(char opening, char closing) {
    return (opening == '(' && closing == ')') ||
           (opening == '[' && closing == ']') ||
           (opening == '{' && closing == '}');
}
int isValid(const char text[]) {
    Stack stack; initStack(&stack);
    for (int i = 0; text[i]; ++i) {
        char ch = text[i];
        if (ch == '(') { push(&stack, ch); }
        else {
            if (isEmpty(&stack)) return 0;
            char opening = (char)pop(&stack);
            if (!matches(opening, ch)) return 0;
        }
    }
    return isEmpty(&stack);
}
int main(void) {
    char text[10000]; if (scanf("%9999s", text) != 1) return 0;
    printf("%s\n", isValid(text) ? "yes" : "no"); return 0;
}
