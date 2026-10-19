# Problem files

One JSON file per problem: `Q01.json` ... `Q15.json`. Copy `Q01.json` as your template.
Authors: Atharv (Q01-Q08), Priyanshi (Q09-Q15). Code that reads these files: Riddhima (`QuestionBank`).

JSON has no comments, so this table is the documentation.

| Field | Required | Meaning |
| --- | --- | --- |
| `id` | yes | `Q01` ... `Q15`, same as the file name |
| `title`, `topic` | yes | Shown in the problem list. `topic` is the DSA area (Arrays, Strings, Stacks ...) |
| `conceptId` | yes | Must exist in `guidance_engine/ConceptGraph.cpp`. If your concept is new, tell Priyanshi |
| `difficulty` | yes | 1 (easiest) to 5 |
| `timeLimitMs` | yes | Per test case. Exceeding it is a runtime error |
| `expectedLoopDepth` | no | 1 for one loop, 2 for nested loops. Used to flag slower-than-expected solutions |
| `statement`, `inputFormat`, `outputFormat`, `example` | yes | What the student reads. `example` is shown on the page |
| `testCases[]` | yes, 5 or more | `id` (`T1`...), `input`, `expected`, `purpose` (one line on what it catches), `visible` (only `true` ones are shown to students) |
| `mistakes[]` | yes, 2 or more | See below |
| `approaches[]` | no | Accepted approaches with time and space complexity, and the reference file |

Each entry of `mistakes[]`:

| Field | Meaning |
| --- | --- |
| `category` | `incorrect_initialization`, `wrong_condition`, `loop_boundary_error` (more can be added: tell Riddhima and Tamnna) |
| `priority` | 1 is checked first. When two mistakes match, the lower number wins |
| `title`, `whyItHappens` | Friendly name and the concept explanation shown to the student |
| `detect.anyPattern` | C++ regular expressions run on the student's code with comments removed. Any one matching counts |
| `detect.failsAnyOf` | Test ids. The mistake is reported only if the pattern matches AND at least one of these tests failed |
| `hint1` | Level 1: a question or a gentle clue |
| `hint2` | Level 2: points at the mistake more directly |
| `wrongExample` | A real wrong program (in `test_cases/<id>/`) that must be classified as this category |

## Rules for good problem files

1. A hint never contains the corrected code or the exact line to change.
2. Every mistake needs a `wrongExample` file that really fails the tests, listed in `test_cases/<id>/manifest.json`.
3. Every test in `failsAnyOf` must really fail for that wrong example. Check with the example program or the verification script.
4. Regexes must allow a type before the variable name (`for (int i = ...`), extra spaces, and a few common variable names.
5. Keep the reference solution in `test_cases/<id>/correct.cpp`. It never goes in the problem file, because the server must not be able to leak it.
6. In JSON a backslash is written twice (`\\s` for `\s`) and a new line is `\n`.

`GET /api/problems/:id` must send the browser only `id`, `title`, `difficulty`, `concept`, `statement`, `inputFormat`, `outputFormat` and `example` (today it sends the first five; Priyanshi adds the rest when the loader lands). Hidden tests, patterns, hints and solutions stay on the server.
