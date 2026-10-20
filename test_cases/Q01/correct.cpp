// Reference solution for Q01 - NEVER shown to students.
#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;

    int a[1000];
    for (int i = 0; i < n; i++)
        cin >> a[i];

    int best = a[0];            // start from a real element
    for (int i = 1; i < n; i++) // visit every remaining element
    {
        if (a[i] > best)
            best = a[i];
    }

    cout << best << endl;
    return 0;
}
