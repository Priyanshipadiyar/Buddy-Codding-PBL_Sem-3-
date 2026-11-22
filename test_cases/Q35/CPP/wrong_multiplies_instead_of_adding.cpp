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

void twoSum(const int arr[], int n, int target, int *firstIndex, int *secondIndex) {
    *firstIndex = -1; *secondIndex = -1;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (arr[i] * arr[j] == target) { *firstIndex = i; *secondIndex = j; return; }
        }
    }
}
int main(void) {
    int n, target, arr[10000], firstIndex, secondIndex;
    if (scanf("%d%d", &n, &target) != 2) return 0;
    readArray(arr, n); twoSum(arr, n, target, &firstIndex, &secondIndex);
    printf("%d %d\n", firstIndex, secondIndex); return 0;
}
