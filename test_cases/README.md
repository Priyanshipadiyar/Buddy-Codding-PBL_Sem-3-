# Buddy Coding: rebuilt modular ADT dataset (Q01–Q35)

## Installation
Extract **problems_json.zip** at the repository root, creating `database/problems/Q01.json` ... `Q35.json`.
Extract **test_cases.zip** at the repository root, creating `test_cases/Q01/` ... `test_cases/Q35/`.

Each question directory has 6 `T#_in.txt` and `T#_out.txt` files, plus C/correct.c, CPP/correct.cpp,
and six language-specific wrong_*.c / wrong_*.cpp examples.

## Input contract
Raw stdin is used with the exact JSON T1..T6 input. Q07 interprets prefixA-size, prefixB-size,
shared-tail-size, then physically connects both lists to the same shared nodes. Q28 interprets
`n cyclePos` followed by n values, with cyclePos=-1 meaning no cycle. Q30 takes `push x` /
`pop` operations until EOF. Q31 is a BST insertion stream. Q32 uses `-1` null entries in
level-order slots at indices 2*i+1 and 2*i+2.

## Detection logic
Every mistake has `detect.anyPattern` with literal C/C++ code snippets (no re: syntax,
no comments), and `detect.failsAnyOf` with the IDs that both wrong-language examples fail.
The app should require **both** a code pattern match AND at least one failed test ID in
`failsAnyOf` before issuing the category hint. Higher-priority rules are numbered 1..6.
Seven progressive hints exist per category; increase hint level when the same category persists.

Do not treat static patterns as proof that they diagnose arbitrary unseen student code.
The 420 included wrong programs have exhaustive targeted checks; other implementation styles
will require more patterns and classification tests. Runtime TLE/sanitizer failures need to be
reported distinctly from wrong output. If your engine's whitespace normalization differs,
use the supplied validation logic as a reference and rerun integration tests.

## Verification
`validation_report.json` contains compile/run checks. `verify_dataset.py` can rerun all
references and wrong programs after unzipping both files at your repo root.
