#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <climits>
#include <algorithm>
using std::scanf;
using std::printf;
using std::fgets;
using std::strtok;
using std::atoi;
using std::strcmp;

void readArray(int arr[], int n) {
    for (int i = 0; i < n; ++i) scanf("%d", &arr[i]);
}
void printArray(const int arr[], int n) {
    for (int i = 0; i < n; ++i) printf("%s%d", i ? " " : "", arr[i]);
    printf("\n");
}

void mergeArrays(int nums1[], int m, const int nums2[], int n) {
    int left = m - 1, right = n - 1, write = m + n - 1;
    while (left >= 0 && right >= 0) {
        if (nums1[left] > nums2[right]) nums1[write--] = nums1[left--];
        else nums1[write--] = nums2[right--];
    }
    while (right >= 0) nums1[write--] = nums2[right--];
}
int main(void) {
    int m, n, nums1[20000], nums2[10000];
    if (scanf("%d", &m) != 1) return 0;
    readArray(nums1, m);
    if (scanf("%d", &n) != 1) return 0;
    readArray(nums2, n);
    mergeArrays(nums1, m, nums2, n); printArray(nums1, m + n - (m + n > 0)); return 0;
}
