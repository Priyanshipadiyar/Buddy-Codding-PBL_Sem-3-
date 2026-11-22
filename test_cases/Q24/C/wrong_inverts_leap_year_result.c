#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int isLeapYear(int year) {
    return !((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0));
}
int main(void) {
    int year; if (scanf("%d", &year) != 1) return 0;
    printf("%s\n", isLeapYear(year) ? "yes" : "no"); return 0;
}
