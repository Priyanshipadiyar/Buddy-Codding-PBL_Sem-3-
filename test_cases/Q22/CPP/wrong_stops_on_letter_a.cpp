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

int stringLength(const char text[]) {
    int count = 0;
    while (text[count] != '\0' && text[count] != 'a') ++count;
    return count;
}
int main(void) {
    char text[10000]; if (scanf("%9999s", text) != 1) return 0;
    printf("%d\n", stringLength(text)); return 0;
}
