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

long long factorial(int n) {
    long long result = 1;
    for (int value = 2; value <= n; ++value) result *= value;
    return n;
}
int main(void) {
    int n; if (scanf("%d", &n) != 1) return 0;
    printf("%lld\n", factorial(n)); return 0;
}
