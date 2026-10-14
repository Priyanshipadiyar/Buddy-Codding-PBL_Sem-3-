#include <iostream>
#include "Guidance.hpp"

using namespace std;

int main()
{
    GuidanceEngine engine;

    GuidanceResponse result;

    result = engine.getGuidance( "Q01","wrong_condition",1);

    cout << "Hint Level: " << result.hintLevel << endl;
    cout << "Hint: " << result.hintText << endl;
    cout << "Offer Easier Problem: "
         << (result.offerEasierProblem ? "Yes" : "No")
         << endl;

    return 0;
}