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

Node* mergeLists(Node* l1, Node* l2) {
    Node dummy = {0, nullptr}; Node* tail = &dummy;
    while (l1 && l2) {
        if (l1->data <= l2->data) { tail->next = l1; l1 = l1->next; }
        else { tail->next = l2; l2 = l2->next; }
        tail = tail->next;
    }
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}
int main(void) {
    int n, value; Node* l1 = nullptr; Node* l2 = nullptr;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) { scanf("%d", &value); insertAtEnd(l1, value); }
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) { scanf("%d", &value); insertAtEnd(l2, value); }
    Node* head = mergeLists(l1, l2);
    printList(head); freeList(head); return 0;
}
