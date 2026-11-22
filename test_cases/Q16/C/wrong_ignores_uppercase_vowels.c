#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int isVowel(char ch) {
    ch = ch;
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
}
int countVowels(const char text[]) {
    int count = 0;
    for (int i = 0; text[i] != '\0'; ++i) if (isVowel(text[i])) ++count;
    return count;
}
int main(void) {
    char text[10000]; if (scanf("%9999s", text) != 1) return 0;
    printf("%d\n", countVowels(text)); return 0;
}
