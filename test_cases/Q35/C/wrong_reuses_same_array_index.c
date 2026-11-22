#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
        for (int j = i; j < n; ++j) {
            if (arr[i] + arr[j] == target) { *firstIndex = i; *secondIndex = j; return; }
        }
    }
}
int main(void) {
    int n, target, arr[10000], firstIndex, secondIndex;
    if (scanf("%d%d", &n, &target) != 2) return 0;
    readArray(arr, n); twoSum(arr, n, target, &firstIndex, &secondIndex);
    printf("%d %d\n", firstIndex, secondIndex); return 0;
}
