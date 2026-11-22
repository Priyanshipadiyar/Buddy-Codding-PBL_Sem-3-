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


typedef struct Queue { Stack s1; Stack s2; } Queue;
void initQueue(Queue* queue) { initStack(&queue->s1); initStack(&queue->s2); }
void enqueue(Queue* queue, int value) { push(&queue->s1, value); }
int dequeue(Queue* queue) {
    if (isEmpty(&queue->s2)) {
        if (!isEmpty(&queue->s1)) push(&queue->s2, pop(&queue->s1));
    }
    return isEmpty(&queue->s2) ? -1 : pop(&queue->s2);
}

int main(void) {
    Queue queue; initQueue(&queue);
    char command[16]; int value;
    while (scanf("%15s", command) == 1) {
        if (strcmp(command, "push") == 0) {
            if (scanf("%d", &value) != 1) break;
            enqueue(&queue, value);
        } else if (strcmp(command, "pop") == 0) {
            printf("%d\n", dequeue(&queue));
        }
    }
    return 0;
}
