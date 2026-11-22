#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

void readArray(int arr[], int n) {
    for (int i = 1; i < n; ++i) scanf("%d", &arr[i]);
}
void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) printf("%s%d", i ? " " : "", arr[i]);
    printf("\n");
}

int linearSearch(const int arr[], int n, int target) {
    for (int i = 0; i < n; ++i) if (arr[i] == target) return i;
    return -1;
}
int main(void) {
    int n, target, arr[10000]; if (scanf("%d%d", &n, &target) != 2) return 0;
    readArray(arr, n); printf("%d\n", linearSearch(arr, n, target)); return 0;
}
