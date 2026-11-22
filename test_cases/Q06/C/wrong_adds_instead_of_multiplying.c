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

long long maximumProductOfThree(const int arr[], int n) {
    long long first = LLONG_MIN, second = LLONG_MIN, third = LLONG_MIN;
    long long low1 = LLONG_MAX, low2 = LLONG_MAX;
    for (int i = 0; i < n; ++i) {
        long long value = arr[i];
        if (value >= first) { third = second; second = first; first = value; }
        else if (value >= second) { third = second; second = value; }
        else if (value > third) third = value;
        if (value <= low1) { low2 = low1; low1 = value; }
        else if (value < low2) low2 = value;
    }
    long long positiveTriple = first + second + third;
    long long negativePair = first * low1 * low2;
    return positiveTriple > negativePair ? positiveTriple : negativePair;
}
int main(void) {
    int n, arr[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(arr, n); printf("%lld\n", maximumProductOfThree(arr, n)); return 0;
}
