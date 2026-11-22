#pragma once

#include <string>
#include <stdexcept>

// Language selected by the learner. Keep this enum independent of the UI.
enum class Language {
    C,
    CPP
};

inline Language languageFromString(const std::string& value) {
    if (value == "C" || value == "c") return Language::C;
    if (value == "C++" || value == "cpp" || value == "CPP") return Language::CPP;
    throw std::invalid_argument("Unsupported language. Choose C or C++.");
}

inline const char* languageName(Language language) {
    return language == Language::C ? "C" : "C++";
}
