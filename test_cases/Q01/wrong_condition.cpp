// Known-wrong submission for Q01: wrong_condition
#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;

    int a[1000];
    for (int i = 0; i < n; i++)
        cin >> a[i];

    int best = a[0];
    for (int i = 1; i < n; i++)
    {
        if (a[i] < best)
            best = a[i];
    }

    cout << best << endl;
    return 0;
}
