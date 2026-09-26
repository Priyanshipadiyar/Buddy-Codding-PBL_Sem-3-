# Guidance Engine Module

**Owner:** Member 3 - Tamanna
**Language:** C++

---

## 📌 Responsibilities

The Guidance Engine provides feedback to the student based on the logical mistake identified by the Evaluation Engine and the student's attempt history.

Key responsibilities:
1. Receive `problemId`, `mistakeCategory`, and `attemptCount` from the Integration Engine.
2. Select progressive hints based on the number of attempts:
   - **Level 1 (Attempt 1):** Conceptual clue / gentle prompt.
   - **Level 2 (Attempt 2+):** More direct hint related to the identified logical mistake.
3. Handle mistake categories provided by the Evaluation Engine:
   - `incorrect_initialization`
   - `wrong_condition`
   - `loop_boundary_error`
4. Determine whether the student is stuck:
   - If `attemptCount >= 2`, set `offerEasierProblem = true`.
5. Return `hintLevel`, `hintText`, and `offerEasierProblem` to the Integration Engine.

---

## Interface Specification

The `GuidanceEngine` class is implemented in `Guidance.hpp` and `Guidance.cpp`.

### `Guidance.hpp` (Header File)

```cpp
#pragma once
#include <string>

struct GuidanceResponse
{
    int hintLevel;
    std::string hintText;
    bool offerEasierProblem;
};

class GuidanceEngine
{
public:
    GuidanceResponse getGuidance(
        const std::string& problemId,
        const std::string& mistakeCategory,
        int attemptCount
    );
};
