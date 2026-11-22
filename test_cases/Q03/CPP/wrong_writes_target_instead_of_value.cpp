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

int removeElement(int arr[], int n, int target) {
    int write = 0;
    for (int read = 0; read < n; ++read)
        if (arr[read] != target) arr[write++] = target;
    return write;
}
int main(void) {
    int n, target, arr[10000]; if (scanf("%d%d", &n, &target) != 2) return 0;
    readArray(arr, n); int count = removeElement(arr, n, target);
    printf("%d\n", count); printArray(arr, count); return 0;
}
