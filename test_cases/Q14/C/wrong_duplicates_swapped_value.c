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

void reverseArray(int arr[], int n) {
    int left = 0, right = n - 1;
    while (left < right) {
        int temp = arr[left]; arr[left] = arr[right]; arr[right] = arr[left];
        ++left; --right;
    }
}
int main(void) {
    int n, arr[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(arr, n); reverseArray(arr, n); printArray(arr, n); return 0;
}
