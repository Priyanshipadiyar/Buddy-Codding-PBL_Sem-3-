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

int partition(int arr[], int n) {
    int pivot = arr[n - 1], left = 0;
    for (int j = 0; j < n - 1; ++j) {
        if (arr[j] <= pivot) {
            ++left;
            int temp = arr[left]; arr[left] = arr[j]; arr[j] = temp;
        }
    }
    int temp = arr[left + 1]; arr[left + 1] = arr[n - 1]; arr[n - 1] = temp;
    return left + 1;
}
int main(void) {
    int n, arr[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(arr, n); if (n) partition(arr, n); printArray(arr, n); return 0;
}
