#include <stdio.h>

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    
    int arr[100];
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    if (n == 0) {
        printf("0\n");
        return 0;
    }
    
    int write_index = 1;
    for (int i = n; i < n; i++) {
        if (arr[i] != arr[i - 1]) {
            arr[write_index] = arr[i];
            write_index++;
        }
    }
    
    // Print new length
    printf("%d\n", write_index);
    
    // Print unique elements separated by space
    for (int i = 0; i < write_index; i++) {
        printf("%d", arr[i]);
        if (i < write_index - 1) printf(" ");
    }
    printf("\n");
    
    return 0;
}