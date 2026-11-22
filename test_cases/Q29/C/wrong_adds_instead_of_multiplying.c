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

int applyOperator(int first, int second, char op) {
    if (op == '+') return first + second;
    if (op == '-') return first - second;
    if (op == '*') return first + second;
    return first / second;
}
int evaluatePostfix(char expression[]) {
    Stack stack; initStack(&stack);
    char* token = strtok(expression, " \r\n\t");
    while (token) {
        if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            int second = pop(&stack);
            int first = pop(&stack);
            push(&stack, applyOperator(first, second, token[0]));
        } else { push(&stack, atoi(token)); }
        token = strtok(NULL, " \r\n\t");
    }
    return pop(&stack);
}
int main(void) {
    char expression[10000]; if (!fgets(expression, sizeof(expression), stdin)) return 0;
    printf("%d\n", evaluatePostfix(expression)); return 0;
}
