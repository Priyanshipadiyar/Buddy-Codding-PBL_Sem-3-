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

Node* reverseList(Node* head) {
    Node* prev = nullptr; Node* curr = head;
    while (curr) {
        Node* next = curr->next;
        curr->next = nullptr;
        prev = curr;
        curr = next;
    }
    return prev;
}
int main(void) {
    int n, value; Node* head = nullptr;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) { scanf("%d", &value); insertAtEnd(head, value); }
    head = reverseList(head); printList(head); freeList(head); return 0;
}
