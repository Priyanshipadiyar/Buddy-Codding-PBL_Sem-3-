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

int findMax(const int arr[], int n) {
    int best = 0;
    for (int i = 1; i < n; ++i) if (arr[i] > best) best = arr[i];
    return best;
}
int main(void) {
    int n, arr[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(arr, n); printf("%d\n", findMax(arr, n)); return 0;
}
