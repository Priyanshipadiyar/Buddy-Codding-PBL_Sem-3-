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

void printFibonacci(int n) {
    long long first = 0, second = 1;
    for (int i = 0; i < n; ++i) {
        printf("%s%lld", i ? " " : "", first);
        long long next = first + second; first = second; second = next;
    }
    printf("\n");
}
int main(void) {
    int n; if (scanf("%d", &n) != 1) return 0;
    printFibonacci(n); return 0;
}
