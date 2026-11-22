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

int isPrime(int n) {
    if (n < 2) return 0;
    for (int divisor = 2; divisor <= n / divisor; ++divisor)
        if (n % divisor == 0) return 0;
    return 0;
}
int main(void) {
    int n; if (scanf("%d", &n) != 1) return 0;
    printf("%s\n", isPrime(n) ? "yes" : "no"); return 0;
}
