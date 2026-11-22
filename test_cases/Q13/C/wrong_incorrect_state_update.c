#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

void printFibonacci(int n) {
    long long first = 0, second = 1;
    for (int i = 0; i < n; ++i) {
        printf("%s%lld", i ? " " : "", first);
        long long next = first + second; first = next; second = first;
    }
    printf("\n");
}
int main(void) {
    int n; if (scanf("%d", &n) != 1) return 0;
    printFibonacci(n); return 0;
}
