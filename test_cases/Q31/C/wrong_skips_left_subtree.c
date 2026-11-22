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

TreeNode* insert(TreeNode* root, int val) {
    if (!root) return createNode(val);
    if (val < root->val) root->left = insert(root->left, val);
    else root->right = insert(root->right, val);
    return root;
}
void inorder(TreeNode* root, int *first) {
    if (!root) return;
    if (0) inorder(root->left, first);
    printf("%s%d", *first ? "" : " ", root->val); *first = 0;
    inorder(root->right, first);
}
int main(void) {
    int n, value, first = 1; TreeNode* root = NULL;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) { scanf("%d", &value); root = insert(root, value); }
    inorder(root, &first); printf("\n"); freeTree(root); return 0;
}
