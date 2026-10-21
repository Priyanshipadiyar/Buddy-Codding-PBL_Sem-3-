#include "../Guidance.hpp"
#include <iostream>
#include <cassert>
#include <fstream>

void setupMockDB() {
    std::ofstream out("Q99.json");
    out << R"({
        "mistakes": [
            { "category": "loop_error", "hint1": "Hint 1", "hint2": "Hint 2", "hint3": "Hint 3", "hint4": "Hint 4" },
            { "category": "missing_logic", "hint1": "Only Hint 1" }
        ]
    })";
    out.close();
}

int main() {
    setupMockDB();
    GuidanceEngine engine("./"); // point to current dir

    // Test 1: Level 1
    auto r1 = engine.getGuidance("Q99", "loop_error", 1);
    assert(r1.hintLevel == 1 && r1.hintText == "Hint 1" && !r1.offerEasierProblem);
    std::cout << "Test 1 Passed: Hint 1 retrieved.\n";

    // Test 2: Level 3 (Offers easier problem)
    auto r2 = engine.getGuidance("Q99", "loop_error", 3);
    assert(r2.hintLevel == 3 && r2.hintText == "Hint 3" && r2.offerEasierProblem);
    std::cout << "Test 2 Passed: Hint 3 retrieved and easier problem offered.\n";

    // Test 3: Exhausted Hints (Fallback to previous)
    auto r3 = engine.getGuidance("Q99", "missing_logic", 3);
    assert(r3.hintLevel == 3 && r3.hintText == "Only Hint 1" && r3.offerEasierProblem);
    std::cout << "Test 3 Passed: Hint fallback worked safely.\n";
    
    // Test 4: Unknown mistake
    auto r4 = engine.getGuidance("Q99", "unknown_mistake", 1);
    assert(r4.hintText == "Check your code carefully. Some test cases are failing.");
    std::cout << "Test 4 Passed: Unknown mistake fallback worked safely.\n";

    std::remove("Q99.json");
    return 0;
}
