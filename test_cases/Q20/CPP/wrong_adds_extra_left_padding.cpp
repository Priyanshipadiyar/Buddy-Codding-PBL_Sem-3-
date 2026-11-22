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
