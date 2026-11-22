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

void swapWithoutTemporary(int *left, int *right) {
    if (left == right) return;
    *left ^= *right;
    *right = *left;
    *left ^= *right;
}
int main(void) {
    int left, right; if (scanf("%d%d", &left, &right) != 2) return 0;
    swapWithoutTemporary(&left, &right);
    printf("%d %d\n", left, right); return 0;
}
