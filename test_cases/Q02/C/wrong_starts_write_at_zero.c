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

int removeDuplicates(int arr[], int n) {
    if (n == 0) return 0;
    int write = 0;
    for (int read = 1; read < n; ++read)
        if (arr[read] != arr[write - 1]) arr[write++] = arr[read];
    return write;
}
int main(void) {
    int n, arr[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(arr, n); int count = removeDuplicates(arr, n);
    printf("%d\n", count); if (count) printArray(arr, count); return 0;
}
