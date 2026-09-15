
# Buddy Coding

### Guided Coding Mentor — Finds Your Wrong Reasoning, Not Just Your Wrong Answer

## 📌 Project Overview

**Buddy Coding** is a guided problem-solving system designed to help students improve their programming skills and logical thinking.

Traditional coding platforms mainly tell students whether their submitted code is correct or incorrect. Buddy Coding goes a step further by identifying predefined logical mistakes in the student's approach and providing **progressive hints** to help them understand and correct their reasoning.

The system does not directly reveal the solution. Instead, it encourages students to analyze their mistakes, retry their code, and develop independent problem-solving skills.

> **Core Idea:** Don't just tell the student that the answer is wrong — help them understand why their reasoning is wrong.

---

## 🎯 Objectives

- Identify common logical mistakes in student code.
- Provide progressive hints instead of directly revealing solutions.
- Encourage independent problem solving.
- Help students understand the concepts behind their mistakes.
- Provide an easier, related problem when a student cannot start.
- Support multiple programming languages.
- Apply concepts of **Data Structures & Algorithms, OOP, and software engineering**.

---

## 🔄 System Workflow

```text
        User Selects Problem
                ↓
      Select Programming Language
                ↓
          Submit Code
                ↓
         Compile the Code
                ↓
         Execute Test Cases
                ↓
       Compare Expected Output
       with Actual Output
                ↓
          ┌─────┴─────┐
          ↓           ↓
       Correct     Incorrect
          ↓           ↓
       Success    Error Analysis
                      ↓
              ┌───────┴────────┐
              ↓                ↓
       Compile/Runtime     Logical Error
           Error                ↓
              ↓          Identify Mistake
           Feedback             ↓
                         Give Progressive Hint
                                ↓
                              Retry
                                ↓
                    Still Unable to Start?
                                ↓
                       Easier Related Problem
                                ↓
                         Practice & Retry
                                ↓
                       Return to Original Problem
```

---

## 🧩 Main Modules

### 1. Problem & Knowledge Database

This module contains the knowledge required by the system for each problem.

It includes:

* Problem statements
* DSA topics
* Difficulty levels
* Multiple approaches where applicable
* Test cases
* Expected outputs
* Common logical mistakes
* Progressive hints
* Related easier problems

The initial MVP will contain **15 DSA problems**.

---

### 2. Code Execution & Error Detection

This module is responsible for processing the code submitted by the user.

It handles:

* Code compilation
* Code execution
* Test-case execution
* Expected vs. actual output comparison
* Compilation error detection
* Runtime error detection
* Wrong-answer detection
* Predefined logical mistake detection

The initial version will use a **rule-based approach** for identifying known logical mistake patterns.

---

### 3. Guidance Engine

The Guidance Engine determines what feedback should be shown to the student.

It provides:

* Progressive hints
* Conceptual clues
* Retry-based guidance
* Stronger hints after repeated incorrect attempts
* Related easier questions when the student cannot start

The system is designed to guide the student toward the solution rather than directly displaying the final answer.

---

### 4. Frontend & System Integration

The frontend provides the user interface through which students interact with the system.

It includes:

* Problem selection
* Problem statement display
* Programming language selection
* Code editor
* Code submission
* Error/result display
* Hint display
* Retry functionality
* Easier-question navigation

This module also integrates the database, code execution module, logic detection module, and guidance engine.

---

## 🧠 Logical Mistake Categories

The initial rule-based system can identify predefined categories of logical mistakes, including:

* Incorrect initialization
* Incorrect conditions
* Boundary and index errors
* Loop errors
* Incorrect variable updates
* Incorrect algorithm or approach
* Missing edge-case handling

These categories can be expanded as more problems and mistake patterns are added.

---

## 📚 DSA Concepts Covered

The project can demonstrate concepts including:

* Arrays
* Strings
* Searching
* Sorting
* Linked Lists
* Stacks
* Queues
* Recursion
* Hashing
* Trees
* Graphs
* Time Complexity
* Space Complexity

---

## 🛠️ Technologies

The technology stack will be finalized during development.

The planned technologies may include:

* **Programming Languages:** C++, Python, Java / C
* **Frontend:** HTML, CSS, JavaScript
* **Backend:** Python
* **Database:** JSON / SQLite
* **Version Control:** Git & GitHub

---

## 👥 Team Structure

| Team Member              | Responsibility                               |
| ------------------------ | -------------------------------------------- |
| **Member 1**             | Problem & Knowledge Database                 |
| **Member 2**             | Code Execution & Logic/Error Detection       |
| **Member 3**             | Guidance Engine                              |
| **Member 4 – Team Lead** | Frontend, Integration & Project Coordination |

---

## 📂 Project Structure

```text
Buddy-Coding/
│
├── frontend/
│
├── backend/
│
├── database/
│   ├── problems/
│   └── README.md
│
├── logic_engine/
│
├── guidance_engine/
│
├── test_cases/
│
├── docs/
│
├── README.md
│
├── .gitignore
│
└── requirements.txt
```

---

## 🔗 Module Communication

The project modules will communicate using structured data.

### Example: Code Analysis Result

```json
{
  "problem_id": "Q01",
  "language": "cpp",
  "status": "wrong_answer",
  "error_type": "logic_error",
  "mistake_category": "wrong_condition"
}
```

### Example: Guidance Response

```json
{
  "hint_level": 1,
  "hint": "Think about the condition used to update the maximum."
}
```

This common structure will make integration between the different team modules easier.

---

## 🚀 Development Plan

### Phase 1 — Foundation

* Finalize project architecture
* Finalize the 15 DSA problems
* Create database structure
* Define common data formats
* Set up GitHub repository
* Create basic frontend
* Create initial code execution prototype

### Phase 2 — Core Development

* Add problem data
* Add test cases
* Implement code compilation and execution
* Implement error classification
* Implement predefined logical mistake detection
* Implement progressive hints
* Connect frontend and backend

### Phase 3 — Integration

* Integrate all modules
* Implement retry mechanism
* Implement easier related problems
* Add support for multiple programming languages
* Test the complete workflow

### Phase 4 — Testing & Documentation

* Test all 15 problems
* Test different approaches
* Test edge cases
* Fix bugs
* Complete project documentation
* Prepare demonstration, PPT and viva material

---

## 📌 MVP Scope

The first working version of Buddy Coding will contain:

* **15 DSA problems**
* **2–3 programming languages**
* Multiple approaches where applicable
* Predefined logical mistake detection
* Compilation and runtime error detection
* Test-case evaluation
* Progressive hints
* Retry mechanism
* Easier related problems
* Integrated frontend and backend

---

## 🔮 Future Scope

The current MVP uses a **rule-based approach** with manually defined logical mistake patterns.

In future versions, the system can be enhanced using **AI/ML techniques** to analyze code and reasoning more dynamically.

Possible future improvements include:

* AI-powered reasoning analysis
* Automatic logical mistake detection
* Personalized learning paths
* Adaptive problem difficulty
* Automatic hint generation
* Support for additional programming languages
* Student performance analytics
* Progress tracking
* Larger problem database
* Personalized recommendations based on previous mistakes

---

## 🎓 Academic Project

**Buddy Coding — Guided Problem Solving System**

A **3rd Semester Group Project** focused on:

* Data Structures & Algorithms
* Object-Oriented Programming
* Logical Problem Solving
* Software Engineering
* Version Control and Collaborative Development

---

## 📍 Current Status

**Development Stage:** Initial MVP Development

The project is currently being developed as a collaborative four-member team project. The initial goal is to build a functional rule-based guided coding system before exploring AI/ML-based enhancements.

---

### 💡 Project Tagline

> **Buddy Coding — Understand Your Mistake. Improve Your Reasoning. Solve It Yourself.**
