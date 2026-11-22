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

Node* getIntersection(Node* headA, Node* headB) {
    Node* currA = headA; Node* currB = headB;
    while (currA != currB) {
        currA = currA ? currA->next : headB;
        currB = currB ? currB->next : headA;
    }
    return currA;
}
Node* appendShared(Node* head, Node* shared) {
    if (!head) return shared;
    Node* curr = head;
    while (curr->next) curr = curr->next;
    curr->next = shared; return head;
}
void freePrefix(Node* head, Node* shared) {
    while (head != shared) { Node* next = head->next; free(head); head = next; }
}
int main(void) {
    int aCount, bCount, sharedCount, value;
    if (scanf("%d%d%d", &aCount, &bCount, &sharedCount) != 3) return 0;
    Node* headA = NULL; Node* headB = NULL; Node* shared = NULL;
    for (int i = 0; i < aCount; ++i) { scanf("%d", &value); headA = insertEnd(headA, value); }
    for (int i = 0; i < bCount; ++i) { scanf("%d", &value); headB = insertEnd(headB, value); }
    for (int i = 0; i < sharedCount; ++i) { scanf("%d", &value); shared = insertEnd(shared, value); }
    headA = appendShared(headA, shared); headB = appendShared(headB, shared);
    Node* intersection = getIntersection(headA, headB);
    printf("%s\n", intersection ? "intersect_node" : "null");
    freePrefix(headA, shared); freePrefix(headB, shared); freeList(shared);
    return 0;
}
