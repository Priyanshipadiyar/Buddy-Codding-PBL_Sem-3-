// Known-wrong submission for Q01: incorrect_initialization
#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;

    int max = 0;
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        if (x > max)
            max = x;
    }

    cout << max << endl;
    return 0;
}
