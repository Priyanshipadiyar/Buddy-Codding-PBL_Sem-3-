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

Node* findMiddle(Node* head) {
    Node* slow = head; Node* fast = head;
    while (fast && fast->next) {
        slow = slow->next ? slow->next->next : slow->next;
        fast = fast->next->next;
    }
    return slow;
}
int main(void) {
    int n, value; Node* head = nullptr;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) { scanf("%d", &value); insertAtEnd(head, value); }
    Node* middle = findMiddle(head);
    if (middle) printf("%d\n", middle->data);
    freeList(head); return 0;
}
