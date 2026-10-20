#include "../LogicEngine.hpp"
#include <iostream>
#include <cassert>
#include <fstream>

void setupMockDB() {
    std::ofstream out("Q99.json");
    out << R"({
        "mistakes": [
            { "category": "incorrect_initialization", "priority": 1, "title": "Wrong init", "detect": { "anyPattern": ["= 0;"], "failsAnyOf": ["T2"] } },
            { "category": "loop_boundary_error", "priority": 2, "title": "Loop error", "detect": { "anyPattern": ["< n - 1"], "failsAnyOf": ["T1"] } }
        ]
    })";
    out.close();
}

int main() {
    setupMockDB();
    LogicEngine engine("./"); // current dir for DB
    
    // Test 1: Match Priority 1
    auto r1 = engine.evaluate("Q99", "int max = 0;", {"T2"});
    assert(r1.classified == true);
    assert(r1.categoryString == "incorrect_initialization");
    std::cout << "Test 1 Passed: Priority 1 matched.\n";

    // Test 2: Match Priority 2
    auto r2 = engine.evaluate("Q99", "for(int i=0; i < n - 1; i++)", {"T1"});
    assert(r2.classified == true);
    assert(r2.categoryString == "loop_boundary_error");
    std::cout << "Test 2 Passed: Priority 2 matched.\n";

    // Test 3: Unmatched Test
    auto r3 = engine.evaluate("Q99", "int max = -999;", {"T3"});
    assert(r3.classified == false);
    std::cout << "Test 3 Passed: Unknown error handled safely.\n";
    
    std::remove("Q99.json");
    return 0;
}
