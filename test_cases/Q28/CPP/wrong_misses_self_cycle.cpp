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

bool hasCycle(Node* head) {
    Node* slow = head; Node* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return slow != head;
    }
    return 0;
}
int main(void) {
    int n, cyclePos, value;
    if (scanf("%d%d", &n, &cyclePos) != 2) return 0;
    Node* head = nullptr; Node* nodes[10000];
    for (int i = 0; i < n; ++i) { scanf("%d", &value); insertAtEnd(head, value); }
    Node* curr = head;
    for (int i = 0; i < n; ++i) { nodes[i] = curr; curr = curr->next; }
    if (n > 0 && cyclePos >= 0 && cyclePos < n) nodes[n - 1]->next = nodes[cyclePos];
    printf("%s\n", hasCycle(head) ? "yes" : "no");
    if (n > 0) nodes[n - 1]->next = nullptr;
    freeList(head); return 0;
}
