#include "CodeCheckerAdapter.hpp"
#include <fstream>
#include <sstream>
#include <cstdlib> // Required for system()

// Helper function to read file output (like compile errors or run outputs) into a string
static std::string readFileToString(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string CodeCheckerAdapter::compile(const std::string& sourcePath, const std::string& binaryPath) {
    // We construct a command to compile ONLY the student file.
    // 2> compile_errors.txt redirects any syntax errors into a text file.
    std::string command = "g++ " + sourcePath + " -o " + binaryPath + " 2> compile_errors.txt";
    
    int compileStatus = system(command.c_str());
    
    if (compileStatus != 0) {
        // If it fails, read the error file and return it so the student sees the syntax error
        return readFileToString("compile_errors.txt");
    }
    
    return "Success";
}

std::string CodeCheckerAdapter::run(const std::string& binaryPath, const std::string& inputPath) {
    // Execute the compiled student binary.
    // '< inputPath' feeds the test case input.
    // '> student_output.txt' captures the program's output.
    std::string command = "./" + binaryPath + " < " + inputPath + " > student_output.txt";
    
    int runStatus = system(command.c_str());
    
    // Read the actual output and send it back to the Controller
    return readFileToString("student_output.txt");
}