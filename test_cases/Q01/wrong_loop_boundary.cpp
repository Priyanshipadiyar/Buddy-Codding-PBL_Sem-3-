// Known-wrong submission for Q01: loop_boundary_error
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
    for (int i = 1; i < n - 1; i++)
    {
        if (a[i] > best)
            best = a[i];
    }

    cout << best << endl;
    return 0;
}
