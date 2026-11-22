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

int sumDigits(long long number) {
    if (number < 0) number = -number;
    int sum = 0;
    while (number > 0) { sum += (int)(number / 10); number /= 10; }
    return sum;
}
int main(void) {
    long long n; if (scanf("%lld", &n) != 1) return 0;
    printf("%d\n", sumDigits(n)); return 0;
}
