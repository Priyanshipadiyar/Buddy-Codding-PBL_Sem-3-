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

TreeNode* insert(TreeNode* root, int val) {
    if (!root) return createNode(val);
    if (val < root->val) root->left = insert(root->left, val);
    else root->right = insert(root->right, val);
    return root;
}
void inorder(TreeNode* root, int *first) {
    if (!root) return;
    inorder(root->left, first);
    printf("%s%d", *first ? "" : " ", root->val); *first = 0;
    if (0) inorder(root->right, first);
}
int main(void) {
    int n, value, first = 1; TreeNode* root = nullptr;
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; ++i) { scanf("%d", &value); root = insert(root, value); }
    inorder(root, &first); printf("\n"); freeTree(root); return 0;
}
