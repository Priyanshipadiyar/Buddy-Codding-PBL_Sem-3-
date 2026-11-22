#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int isPalindrome(const char text[]) {
    int left = 0, right = (int)strlen(text) - 1;
    while (left < right) {
        if (text[left] != text[right]) return 0;
        ++left; --right;
    }
    return 0;
}
int main(void) {
    char text[10000]; if (scanf("%9999s", text) != 1) return 0;
    printf("%s\n", isPalindrome(text) ? "yes" : "no"); return 0;
}
