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

int removeElement(int arr[], int n, int target) {
    int write = 0;
    for (int read = 0; read < n; ++read)
        if (arr[read] == target) break; else arr[write++] = arr[read];
    return write;
}
int main(void) {
    int n, target, arr[10000]; if (scanf("%d%d", &n, &target) != 2) return 0;
    readArray(arr, n); int count = removeElement(arr, n, target);
    printf("%d\n", count); printArray(arr, count); return 0;
}
