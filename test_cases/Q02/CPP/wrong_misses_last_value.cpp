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

int removeDuplicates(int arr[], int n) {
    if (n == 0) return 0;
    int write = 1;
    for (int read = 1; read < n - 1; ++read)
        if (arr[read] != arr[write - 1]) arr[write++] = arr[read];
    return write;
}
int main(void) {
    int n, arr[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(arr, n); int count = removeDuplicates(arr, n);
    printf("%d\n", count); if (count) printArray(arr, count); return 0;
}
