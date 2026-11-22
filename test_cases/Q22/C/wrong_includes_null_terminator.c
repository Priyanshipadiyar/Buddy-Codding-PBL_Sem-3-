#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

int stringLength(const char text[]) {
    int count = 0;
    while (text[count] != '\0') ++count;
    return count + 1;
}
int main(void) {
    char text[10000]; if (scanf("%9999s", text) != 1) return 0;
    printf("%d\n", stringLength(text)); return 0;
}
