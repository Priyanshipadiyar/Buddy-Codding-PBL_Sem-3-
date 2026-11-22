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

int maxProfit(const int prices[], int n) {
    if (n < 2) return 0;
    int minPrice = 0, bestProfit = 0;
    for (int i = 1; i < n; ++i) {
        int profit = prices[i] - minPrice;
        if (profit > bestProfit) bestProfit = profit;
        if (prices[i] < minPrice) minPrice = prices[i];
    }
    return bestProfit;
}
int main(void) {
    int n, prices[10000]; if (scanf("%d", &n) != 1) return 0;
    readArray(prices, n); printf("%d\n", maxProfit(prices, n)); return 0;
}
