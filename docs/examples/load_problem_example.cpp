// Example: how C++ reads a problem file and checks a student's source against its
// "detect" patterns. Riddhima's QuestionBank and Evaluator do the same thing for real.
//
// Build (from the repo root):
//   g++ -std=c++17 -Ibackend/third_party docs/examples/load_problem_example.cpp -o load_example
// Run:
//   ./load_example database/problems/Q01.json test_cases/Q01/wrong_init_zero.cpp

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "json.hpp"

using json = nlohmann::json;

// Remove // and /* */ comments so a comment cannot trigger or hide a pattern.
static std::string stripComments(const std::string& src)
{
    std::string out = std::regex_replace(src, std::regex(R"(/\*[\s\S]*?\*/)"), "");
    return std::regex_replace(out, std::regex(R"(//[^\n]*)"), "");
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "usage: " << argv[0] << " problem.json student.cpp\n";
        return 2;
    }

    // 1. Load the problem file.
    std::ifstream pf(argv[1]);
    json problem = json::parse(pf);
    std::cout << problem["id"].get<std::string>() << " - " << problem["title"].get<std::string>()
              << "  (concept " << problem["conceptId"].get<std::string>() << ", difficulty "
              << problem["difficulty"] << ", " << problem["testCases"].size() << " tests)\n";

    // 2. Load the student's source.
    std::ifstream sf(argv[2]);
    std::stringstream ss;
    ss << sf.rdbuf();
    std::string code = stripComments(ss.str());

    // 3. Try each mistake in priority order (1 = checked first).
    std::vector<json> mistakes(problem["mistakes"].begin(), problem["mistakes"].end());
    std::sort(mistakes.begin(), mistakes.end(),
              [](const json& a, const json& b) { return a["priority"] < b["priority"]; });

    for (const json& m : mistakes)
    {
        for (const std::string pattern : m["detect"]["anyPattern"])
        {
            if (std::regex_search(code, std::regex(pattern)))
            {
                std::cout << "pattern matched -> " << m["category"].get<std::string>() << "\n"
                          << "  the Evaluator would now also check that one of "
                          << m["detect"]["failsAnyOf"].dump() << " failed\n"
                          << "  hint 1: " << m["hint1"].get<std::string>() << "\n"
                          << "  hint 2: " << m["hint2"].get<std::string>() << "\n";
                return 0;
            }
        }
    }
    std::cout << "no mistake pattern matched\n";
    return 0;
}
