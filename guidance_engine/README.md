# Buddy Coding - Guidance Engine

## Responsibility
Accepts a detected mistake category and attempt count, and maps it to progressive hints from the JSON rule base.
*NO AI OR ML IS USED IN THIS MODULE.*

## Input Contract (getGuidance)
- `problemId`: string
- `mistakeCategory`: string (Raw string like "incorrect_initialization")
- `attemptCount`: int

## Output Contract (GuidanceResponse)
- `hintLevel`: int (1 to 4)
- `hintText`: string
- `offerEasierProblem`: bool (Triggers at attempt >= 3)

## Build Instructions (Windows/Linux)
1. `mkdir build && cd build`
2. `cmake ..`
3. `cmake --build .`
4. Run tests: `./guidance_test`
