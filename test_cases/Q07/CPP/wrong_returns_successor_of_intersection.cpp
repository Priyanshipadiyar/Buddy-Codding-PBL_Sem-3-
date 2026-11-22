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

struct Node { int data; Node* next; };
Node* createNode(int val) { return new Node{val, nullptr}; }
void insertAtEnd(Node*& head, int val) {
    Node* node = createNode(val);
    if (!head) { head = node; return; }
    Node* curr = head;
    while (curr->next) curr = curr->next;
    curr->next = node;
}
void printList(const Node* head) {
    const Node* curr = head; bool first = true;
    while (curr) { printf("%s%d", first ? "" : " ", curr->data); first = false; curr = curr->next; }
    printf("\n");
}
void freeList(Node* head) {
    while (head) { Node* next = head->next; delete head; head = next; }
}

Node* getIntersection(Node* headA, Node* headB) {
    Node* currA = headA; Node* currB = headB;
    while (currA != currB) {
        currA = currA ? currA->next : headB;
        currB = currB ? currB->next : headA;
    }
    return currA ? currA->next : currA;
}
Node* appendShared(Node* head, Node* shared) {
    if (!head) return shared;
    Node* curr = head;
    while (curr->next) curr = curr->next;
    curr->next = shared; return head;
}
void freePrefix(Node* head, Node* shared) {
    while (head != shared) { Node* next = head->next; delete head; head = next; }
}
int main(void) {
    int aCount, bCount, sharedCount, value;
    if (scanf("%d%d%d", &aCount, &bCount, &sharedCount) != 3) return 0;
    Node* headA = nullptr; Node* headB = nullptr; Node* shared = nullptr;
    for (int i = 0; i < aCount; ++i) { scanf("%d", &value); insertAtEnd(headA, value); }
    for (int i = 0; i < bCount; ++i) { scanf("%d", &value); insertAtEnd(headB, value); }
    for (int i = 0; i < sharedCount; ++i) { scanf("%d", &value); insertAtEnd(shared, value); }
    headA = appendShared(headA, shared); headB = appendShared(headB, shared);
    Node* intersection = getIntersection(headA, headB);
    printf("%s\n", intersection ? "intersect_node" : "null");
    freePrefix(headA, shared); freePrefix(headB, shared); freeList(shared);
    return 0;
}
