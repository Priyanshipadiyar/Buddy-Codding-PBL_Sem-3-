#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

void printPyramid(int height) {
    for (int row = 1; row <= height; ++row) {
        for (int space = 0; space < height - row + 1; ++space) putchar(' ');
        for (int star = 0; star < 2 * row - 1; ++star) putchar('*');
        putchar('\n');
    }
}
int main(void) {
    int height; if (scanf("%d", &height) != 1) return 0;
    printPyramid(height); return 0;
}
