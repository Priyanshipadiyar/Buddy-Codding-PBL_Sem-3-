#include "CodeCheckerAdapter.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

static bool writeFile(const fs::path& path, const std::string& content) {
    std::ofstream file(path, std::ios::binary);
    file << content;
    return static_cast<bool>(file);
}

int main() {
    const fs::path dir = fs::temp_directory_path() / "buddy_language_adapter_test";
    fs::create_directories(dir);
    const fs::path input = dir / "input.txt";
    const fs::path cSource = dir / "student.c";
    const fs::path cppSource = dir / "student.cpp";
#ifdef _WIN32
    const fs::path cBinary = dir / "student_c.exe";
    const fs::path cppBinary = dir / "student_cpp.exe";
#else
    const fs::path cBinary = dir / "student_c";
    const fs::path cppBinary = dir / "student_cpp";
#endif
    writeFile(input, "");
    writeFile(cSource, "#include <stdio.h>\nint main(void) { puts(\"C_OK\"); return 0; }\n");
    writeFile(cppSource, "#include <iostream>\nint main() { std::cout << \"CPP_OK\\n\"; }\n");

    CodeCheckerAdapter adapter;
    const std::string cCompile = adapter.compile(cSource.string(), cBinary.string(), Language::C);
    if (cCompile != "Success") { std::cerr << "C compile failed: " << cCompile << '\n'; fs::remove_all(dir); return 1; }
    const std::string cOutput = adapter.run(cBinary.string(), input.string());
    if (cOutput.find("C_OK") == std::string::npos) { std::cerr << "C run failed: " << cOutput << '\n'; fs::remove_all(dir); return 1; }

    const std::string cppCompile = adapter.compile(cppSource.string(), cppBinary.string(), Language::CPP);
    if (cppCompile != "Success") { std::cerr << "C++ compile failed: " << cppCompile << '\n'; fs::remove_all(dir); return 1; }
    const std::string cppOutput = adapter.run(cppBinary.string(), input.string());
    if (cppOutput.find("CPP_OK") == std::string::npos) { std::cerr << "C++ run failed: " << cppOutput << '\n'; fs::remove_all(dir); return 1; }

    fs::remove_all(dir);
    std::cout << "Language adapter test passed: GCC C11 and G++ C++17 compile and run.\n";
    return 0;
}
