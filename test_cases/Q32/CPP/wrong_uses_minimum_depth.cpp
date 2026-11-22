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

struct TreeNode { int val; TreeNode* left; TreeNode* right; };
TreeNode* createNode(int val) { return new TreeNode{val, nullptr, nullptr}; }
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left); freeTree(root->right); delete root;
}

void readArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) scanf("%d", &arr[i]);
}
void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) printf("%s%d", i ? " " : "", arr[i]);
    printf("\n");
}

TreeNode* buildTree(const int slots[], int n) {
    if (n <= 0 || slots[0] == -1) return nullptr;
    TreeNode* nodes[10000] = {0};
    for (int i = 0; i < n; ++i)
        if (slots[i] != -1 && (i == 0 || nodes[(i - 1) / 2])) nodes[i] = createNode(slots[i]);
    for (int i = 0; i < n; ++i) {
        if (!nodes[i]) continue;
        int leftIndex = 2 * i + 1, rightIndex = leftIndex + 1;
        if (leftIndex < n) nodes[i]->left = nodes[leftIndex];
        if (rightIndex < n) nodes[i]->right = nodes[rightIndex];
    }
    return nodes[0];
}
int maxDepth(TreeNode* root) {
    if (!root) return 0;
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    return 1 + (leftDepth < rightDepth ? leftDepth : rightDepth);
}
int main(void) {
    int n, slots[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(slots, n); TreeNode* root = buildTree(slots, n);
    printf("%d\n", maxDepth(root)); freeTree(root); return 0;
}
