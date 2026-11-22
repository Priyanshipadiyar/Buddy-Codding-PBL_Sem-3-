#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
