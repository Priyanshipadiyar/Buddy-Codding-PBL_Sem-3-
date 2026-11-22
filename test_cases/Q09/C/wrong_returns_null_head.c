#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

typedef struct Node { int data; struct Node* next; } Node;
Node* createNode(int val) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) exit(1);
    node->data = val; node->next = NULL; return node;
}
Node* insertEnd(Node* head, int val) {
    Node* node = createNode(val);
    if (!head) return node;
    Node* curr = head;
    while (curr->next) curr = curr->next;
    curr->next = node; return head;
}
void printList(const Node* head) {
    const Node* curr = head; int first = 1;
    while (curr) { printf("%s%d", first ? "" : " ", curr->data); first = 0; curr = curr->next; }
    printf("\n");
}
void freeList(Node* head) {
    while (head) { Node* next = head->next; free(head); head = next; }
}

Node* reverseList(Node* head) {
    Node* prev = NULL; Node* curr = head;
    while (curr) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return NULL;
}
int main(void) {
    int n, value; Node* head = NULL;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) { scanf("%d", &value); head = insertEnd(head, value); }
    head = reverseList(head); printList(head); freeList(head); return 0;
}
