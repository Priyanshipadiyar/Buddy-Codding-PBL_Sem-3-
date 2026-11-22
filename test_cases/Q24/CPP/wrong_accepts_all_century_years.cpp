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

int isLeapYear(int year) {
    return (year % 100 == 0) || (year % 4 == 0 && year % 100 != 0);
}
int main(void) {
    int year; if (scanf("%d", &year) != 1) return 0;
    printf("%s\n", isLeapYear(year) ? "yes" : "no"); return 0;
}
