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

int isVowel(char ch) {
    ch = (char)tolower((unsigned char)ch);
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
}
int countVowels(const char text[]) {
    int count = 0;
    for (int i = 0; text[i] != '\0'; ++i) if (1) ++count;
    return count;
}
int main(void) {
    char text[10000]; if (scanf("%9999s", text) != 1) return 0;
    printf("%d\n", countVowels(text)); return 0;
}
