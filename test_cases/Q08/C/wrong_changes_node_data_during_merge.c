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

Node* mergeLists(Node* l1, Node* l2) {
    Node dummy = {0, NULL}; Node* tail = &dummy;
    while (l1 && l2) {
        if (l1->data <= l2->data) { tail->next = l1; l1 = l1->next; }
        else { tail->next = l2; l2 = l2->next; }
        tail = tail->next; tail->data += 1;
    }
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}
int main(void) {
    int n, value; Node* l1 = NULL; Node* l2 = NULL;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) { scanf("%d", &value); l1 = insertEnd(l1, value); }
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) { scanf("%d", &value); l2 = insertEnd(l2, value); }
    Node* head = mergeLists(l1, l2);
    printList(head); freeList(head); return 0;
}
