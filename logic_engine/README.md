# Buddy Coding - Logic Engine

## Responsibility
Receives compiler results and student source code, applies pre-defined JSON rules using deterministic Regex matches, and returns a classified `LogicResult`. 
*NO AI OR ML IS USED IN THIS MODULE.*

## Input Contract (evaluate)
- `problemId`: (string) e.g. "Q01"
- `sourceCode`: (string) The raw C++ code.
- `failedTestIds`: (vector<string>) Test cases that failed execution.

## Output Contract (LogicResult)
- `problemId`: string
- `categoryEnum`: `MistakeCategory` (Maps to existing backend enum)
- `categoryString`: string (Raw string from JSON)
- `explanation`: string
- `classified`: boolean (True if a rule fired, false if unknown)

## Build Instructions (Windows/Linux)
1. `mkdir build && cd build`
2. `cmake ..`
3. `cmake --build .`
4. Run tests: `./logic_test` (or `logic_test.exe`)
