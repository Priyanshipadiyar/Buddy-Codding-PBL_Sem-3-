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

void bubbleSort(int arr[], int n) {
    for (int pass = 0; pass < n - 1; ++pass) {
        int swapped = 0;
        for (int j = 0; j < n - pass - 2; ++j) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}
int main(void) {
    int n, arr[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(arr, n); bubbleSort(arr, n); printArray(arr, n); return 0;
}
