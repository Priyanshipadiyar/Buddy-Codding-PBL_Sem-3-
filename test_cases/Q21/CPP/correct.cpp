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

void readArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) scanf("%d", &arr[i]);
}
void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) printf("%s%d", i ? " " : "", arr[i]);
    printf("\n");
}

void countEvenOdd(const int arr[], int n, int *even, int *odd) {
    *even = 0; *odd = 0;
    for (int i = 0; i < n; ++i) {
        if (arr[i] % 2 == 0) ++*even;
        else ++*odd;
    }
}
int main(void) {
    int n, arr[10000], even, odd; if (scanf("%d", &n) != 1) return 0;
    readArray(arr, n); countEvenOdd(arr, n, &even, &odd);
    printf("%d %d\n", even, odd); return 0;
}
