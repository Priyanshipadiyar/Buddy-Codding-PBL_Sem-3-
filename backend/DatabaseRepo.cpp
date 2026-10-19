#include "DatabaseRepo.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "json.hpp" // nlohmann/json single header

namespace fs = std::filesystem;
using json = nlohmann::json;

DatabaseRepo::DatabaseRepo(const std::string& db_dir, const std::string& tc_dir)
    : db_dir_(db_dir), tc_dir_(tc_dir) {}

std::vector<std::string> DatabaseRepo::listProblems() {
    std::vector<std::string> problems;
    if (!fs::exists(db_dir_)) return problems;
    for (const auto& entry : fs::directory_iterator(db_dir_)) {
        if (entry.path().extension() == ".json") {
            problems.push_back(entry.path().stem().string());
        }
    }
    
    // Sort the problems sequentially (Q01, Q02, Q03...)
    std::sort(problems.begin(), problems.end());
    
    return problems;
}

bool DatabaseRepo::loadProblem(const std::string& problemId, Problem& outProblem) {
    std::string json_path = db_dir_ + "/" + problemId + ".json";
    if (!fs::exists(json_path)) return false;

    outProblem.id = problemId;
    outProblem.statement = "No statement found.";

    // Parse the JSON file to extract the real problem statement
    try {
        std::ifstream file(json_path);
        if (file.is_open()) {
            json j;
            file >> j;
            if (j.contains("statement")) {
                outProblem.statement = j["statement"].get<std::string>();
            } else if (j.contains("description")) {
                outProblem.statement = j["description"].get<std::string>();
            }
        }
    } catch (...) {
        outProblem.statement = "[Error parsing statement from JSON]";
    }

    // Read test cases directly from the existing test_cases/ folder
    std::string prob_tc_dir = tc_dir_ + "/" + problemId;
    if (fs::exists(prob_tc_dir)) {
        for (const auto& entry : fs::directory_iterator(prob_tc_dir)) {
            std::string filename = entry.path().filename().string();
            if (filename.find("_in.txt") != std::string::npos) {
                std::string tc_id = filename.substr(0, filename.find("_in.txt"));
                TestCase tc;
                tc.id = tc_id;
                tc.input_path = entry.path().string();
                tc.expected_output_path = prob_tc_dir + "/" + tc_id + "_out.txt";

                if (fs::exists(tc.expected_output_path)) {
                    std::ifstream out_file(tc.expected_output_path);
                    std::stringstream buffer;
                    buffer << out_file.rdbuf();
                    tc.expected_output_content = buffer.str();
                    
                    // Clean trailing newlines to match execution standard
                    while (!tc.expected_output_content.empty() && 
                          (tc.expected_output_content.back() == '\n' || tc.expected_output_content.back() == '\r')) {
                        tc.expected_output_content.pop_back();
                    }
                }
                outProblem.test_cases.push_back(tc);
            }
        }
    }
    return true;
}