#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

long long factorial(int n) {
    long long result = 1;
    for (int value = 2; value <= n; ++value) result += value;
    return result;
}
int main(void) {
    int n; if (scanf("%d", &n) != 1) return 0;
    printf("%lld\n", factorial(n)); return 0;
}
