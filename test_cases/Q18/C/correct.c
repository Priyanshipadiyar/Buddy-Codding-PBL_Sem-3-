#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int isPrime(int n) {
    if (n < 2) return 0;
    for (int divisor = 2; divisor <= n / divisor; ++divisor)
        if (n % divisor == 0) return 0;
    return 1;
}
int main(void) {
    int n; if (scanf("%d", &n) != 1) return 0;
    printf("%s\n", isPrime(n) ? "yes" : "no"); return 0;
}
