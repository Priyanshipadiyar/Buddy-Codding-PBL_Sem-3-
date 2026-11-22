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

void addMatrices(const int first[], const int second[], int result[], int n) {
    for (int row = 0; row < n; ++row)
        for (int col = 0; col < n; ++col)
            result[row * n + col] = first[row * n + col] * second[row * n + col];
}
int main(void) {
    int n, first[10000], second[10000], result[10000] = {0};
    if (scanf("%d", &n) != 1) return 0;
    readArray(first, n * n); readArray(second, n * n);
    addMatrices(first, second, result, n);
    for (int row = 0; row < n; ++row) printArray(result + row * n, n);
    return 0;
}
