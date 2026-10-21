# CodeChecker

A C++ test runner that compiles and evaluates student code against predefined test cases.

## Quick Start

### 1. Build the checker
```bash
g++ -std=c++17 -o CodeChecker.exe src/main.cpp src/FileReader.cpp src/TestCaseLoader.cpp src/Compiler.cpp src/Runner.cpp src/Comparator.cpp src/Reporter.cpp
```

Or using CMake:
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### 2. Set up your inputs in `input/`

- **`input/code.txt`** — Place the C++ source code you want to test
- **`input/qidFile.txt`** — Write the question ID (e.g., `Q01`)

### 3. Run
```bash
./CodeChecker.exe
```

The checker will:
1. Read the source code from `input/code.txt`
2. Read the question ID from `input/qidFile.txt`
3. Find test cases in `testCase/<QID>/` (e.g., `testCase/Q01/`)
4. Compile the code using `g++`
5. Run it against each test case with timeout protection
6. Print a formatted pass/fail report to the terminal
7. Save the test report to **`output/results.txt`**

## Samples Provided

We provide ready-to-test samples in `samples/` and `input/`:

- **Passing all tests:** `samples/pass_all/` or `input/code_sample_pass.txt`
- **Failing some tests:** `samples/fail_some/` or `input/code_sample_fail.txt`

To test the passing solution:
```bash
# In PowerShell:
Copy-Item input/code_sample_pass.txt input/code.txt
./CodeChecker.exe
```

To test the failing solution:
```bash
# In PowerShell:
Copy-Item input/code_sample_fail.txt input/code.txt
./CodeChecker.exe
```

## Output

The test outcome is displayed in the terminal with colored indicators and simultaneously saved to:
- **`output/results.txt`**

## Project Structure

```
CodeChecker/
├── src/                    # Source files
│   ├── main.cpp            # Entry point
│   ├── FileReader.*        # Reads input/code.txt, input/qidFile.txt
│   ├── TestCaseLoader.*    # Discovers test case pairs
│   ├── Compiler.*          # Invokes g++ to compile
│   ├── Runner.*            # Executes binary with test inputs
│   ├── Comparator.*        # Compares actual vs expected output
│   └── Reporter.*          # Prints to console & writes to output/results.txt
├── input/                  # Input directory
│   ├── code.txt            # Current student code to test
│   ├── qidFile.txt         # Question ID
│   ├── code_sample_pass.txt# Sample code that passes all tests
│   └── code_sample_fail.txt# Sample code that fails test T2
├── output/                 # Output directory
│   └── results.txt         # Generated test results report
├── samples/                # Dedicated sample sets
│   ├── pass_all/           # (code.txt, qidFile.txt)
│   └── fail_some/          # (code.txt, qidFile.txt)
├── testCase/               # Test case bank (Q01 to Q09)
├── temp/                   # Scratch dir for compilation & execution
└── CMakeLists.txt          # Build system
```
