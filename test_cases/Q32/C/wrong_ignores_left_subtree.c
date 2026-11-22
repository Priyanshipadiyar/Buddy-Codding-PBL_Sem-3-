#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

typedef struct TreeNode { int val; struct TreeNode* left; struct TreeNode* right; } TreeNode;
TreeNode* createNode(int val) {
    TreeNode* root = (TreeNode*)malloc(sizeof(TreeNode));
    if (!root) exit(1);
    root->val = val; root->left = NULL; root->right = NULL; return root;
}
void freeTree(TreeNode* root) {
    if (!root) return;
    freeTree(root->left); freeTree(root->right); free(root);
}

void readArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) scanf("%d", &arr[i]);
}
void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) printf("%s%d", i ? " " : "", arr[i]);
    printf("\n");
}

TreeNode* buildTree(const int slots[], int n) {
    if (n <= 0 || slots[0] == -1) return NULL;
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
    return 1 + rightDepth;
}
int main(void) {
    int n, slots[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(slots, n); TreeNode* root = buildTree(slots, n);
    printf("%d\n", maxDepth(root)); freeTree(root); return 0;
}
