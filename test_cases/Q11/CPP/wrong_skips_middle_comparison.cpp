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

int isPalindrome(const char text[]) {
    int left = 0, right = (int)strlen(text) - 1;
    while (left + 1 < right) {
        if (text[left] != text[right]) return 0;
        ++left; --right;
    }
    return 1;
}
int main(void) {
    char text[10000]; if (scanf("%9999s", text) != 1) return 0;
    printf("%s\n", isPalindrome(text) ? "yes" : "no"); return 0;
}
