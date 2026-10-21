/* Buddy Coding — Submission and C++ Analyzer */
"use strict";

document.addEventListener("DOMContentLoaded", () => {
    const questions = [
        { id: 1, title: "Find the largest element in an array", level: "Beginner", description: "Given an array of integers, find and display the largest element." },
        { id: 2, title: "Check whether a number is even or odd", level: "Beginner", description: "Read an integer and determine whether it is even or odd." },
        { id: 3, title: "Reverse a number", level: "Beginner", description: "Given an integer, reverse the order of its digits and display the result." },
        { id: 4, title: "Check for a palindrome number", level: "Beginner", description: "Determine whether a number reads the same forwards and backwards." },
        { id: 5, title: "Calculate factorial", level: "Beginner", description: "Read a non-negative integer and calculate its factorial." },
        { id: 6, title: "Print the Fibonacci sequence", level: "Beginner", description: "Print the first N terms of the Fibonacci sequence." },
        { id: 7, title: "Check whether a number is prime", level: "Beginner", description: "Determine whether the given integer is a prime number." },
        { id: 8, title: "Find the sum of array elements", level: "Beginner", description: "Read an array of integers and calculate the sum of all elements." },
        { id: 9, title: "Linear search in an array", level: "Beginner", description: "Search for a target value in an array and report its position if found." },
        { id: 10, title: "Sort an array in ascending order", level: "Intermediate", description: "Arrange the elements of an integer array from smallest to largest." },
        { id: 11, title: "Count vowels in a string", level: "Beginner", description: "Count the vowels present in a given string." },
        { id: 12, title: "Find the second largest element", level: "Intermediate", description: "Find the second-largest distinct value in an integer array." },
        { id: 13, title: "Check balanced parentheses", level: "Intermediate", description: "Determine whether the parentheses in an expression are correctly balanced." },
        { id: 14, title: "Reverse a linked list", level: "Intermediate", description: "Reverse the links of a singly linked list and display the new list." },
        { id: 15, title: "Detect a cycle in a linked list", level: "Intermediate", description: "Determine whether a linked list contains a cycle." }
    ];

    const form = document.getElementById("submissionForm");
    const questionList = document.getElementById("questionList");
    const questionCount = document.getElementById("questionCount");
    const selectedTitle = document.getElementById("selectedTitle");
    const selectedDescription = document.getElementById("selectedDescription");
    const languageSelect = document.getElementById("language");
    const fileExtension = document.getElementById("fileExtension");
    const codeInput = document.getElementById("codeInput");
    const lineNumbers = document.getElementById("lineNumbers");
    const lineCount = document.getElementById("lineCount");
    const clearButton = document.getElementById("clearButton");
    const message = document.getElementById("message");
    const previewPanel = document.getElementById("previewPanel");
    const previewDetails = document.getElementById("previewDetails");
    const previewCode = document.getElementById("previewCode");
    const feedbackElement = previewPanel.querySelector(".notice");

    let selectedQuestion = null;

    function renderQuestions() {
        questionList.innerHTML = "";

        questions.forEach((question) => {
            const label = document.createElement("label");
            label.className = "question-option";
            label.dataset.questionId = String(question.id);

            const radio = document.createElement("input");
            radio.type = "radio";
            radio.name = "selectedQuestion";
            radio.value = String(question.id);
            radio.setAttribute("aria-label", question.title);

            const text = document.createElement("span");
            text.className = "question-text";

            const title = document.createElement("strong");
            title.textContent =
                `${String(question.id).padStart(2, "0")}. ${question.title}`;

            const level = document.createElement("small");
            level.textContent = question.level;

            text.append(title, level);
            label.append(radio, text);
            label.addEventListener("click", () => selectQuestion(question.id));
            questionList.appendChild(label);
        });
    }

    function selectQuestion(id) {
        selectedQuestion =
            questions.find((question) => question.id === id) || null;

        questionList.querySelectorAll(".question-option").forEach((item) => {
            const isSelected = Number(item.dataset.questionId) === id;
            item.classList.toggle("selected", isSelected);
            item.querySelector("input").checked = isSelected;
        });

        if (selectedQuestion) {
            selectedTitle.textContent = selectedQuestion.title;
            selectedDescription.textContent = selectedQuestion.description;
            questionCount.textContent = `${id} of 15 selected`;
        } else {
            selectedTitle.textContent = "Choose a question above";
            selectedDescription.textContent =
                "The question description will appear here.";
            questionCount.textContent = "0 of 15 selected";
        }

        previewPanel.hidden = true;
        message.className = "message";
        message.textContent = "";
    }

    function updateEditorInfo() {
        const count = Math.max(1, codeInput.value.split("\n").length);

        lineNumbers.textContent = Array.from(
            { length: count },
            (_, i) => i + 1
        ).join("\n");

        lineCount.textContent =
            `${count} ${count === 1 ? "line" : "lines"}`;

        fileExtension.textContent =
            languageSelect.value === "cpp" ? "cpp" : "c";
    }

    function showMessage(text, type) {
        message.textContent = text;
        message.className = `message show ${type}`;
    }

    // Connect the frontend to the C++ analyzer backend.
    async function analyzeCode(code) {
        if (languageSelect.value !== "cpp") {
            return "Abhi analyzer sirf C++ ke liye connected hai. C++ select karke try karein.";
        }

        try {
            const response = await fetch(
                "http://localhost:3000/api/analyze",
                {
                    method: "POST",
                    headers: {
                        "Content-Type": "application/json"
                    },
                    body: JSON.stringify({ code })
                }
            );

            const data = await response.json();

            if (!response.ok) {
                return data.report || "Analyzer se feedback nahi mila.";
            }

            return data.report || "No feedback generated.";
        } catch (error) {
            console.error("Analyzer connection error:", error);

            return "Backend connect nahi hua. Check karein ki backend folder mein node server.js chal raha hai.";
        }
    }

    renderQuestions();

    codeInput.addEventListener("input", updateEditorInfo);
    languageSelect.addEventListener("change", updateEditorInfo);

    codeInput.addEventListener("keydown", (event) => {
        if (event.key === "Tab") {
            event.preventDefault();

            const start = codeInput.selectionStart;
            const end = codeInput.selectionEnd;

            codeInput.setRangeText("    ", start, end, "end");
            updateEditorInfo();
        }
    });

    clearButton.addEventListener("click", () => {
        if (!codeInput.value.trim() ||
            window.confirm("Clear all code from the editor?")) {

            codeInput.value = "";
            updateEditorInfo();
            codeInput.focus();
            previewPanel.hidden = true;
            message.className = "message";
            message.textContent = "";
        }
    });

    form.addEventListener("submit", async (event) => {
        event.preventDefault();

        const code = codeInput.value.trim();

        if (!selectedQuestion) {
            showMessage(
                "Please select one of the 15 questions before submitting.",
                "error"
            );
            questionList.scrollIntoView({
                behavior: "smooth",
                block: "center"
            });
            return;
        }

        if (!code) {
            showMessage(
                "Please enter your C or C++ code before submitting.",
                "error"
            );
            codeInput.focus();
            return;
        }

        const submission = {
            id: (window.crypto && crypto.randomUUID)
                ? crypto.randomUUID()
                : String(Date.now()),
            problemId: selectedQuestion.id,
            problemTitle: selectedQuestion.title,
            problemDescription: selectedQuestion.description,
            language: languageSelect.value === "cpp" ? "C++" : "C",
            code,
            submittedAt: new Date().toISOString(),
            status: "Submitted"
        };

        // Save submission in browser history.
        try {
            const key = "buddyCodingSubmissions";
            const existing = JSON.parse(localStorage.getItem(key) || "[]");
            const records = Array.isArray(existing) ? existing : [];

            records.unshift(submission);
            localStorage.setItem(key, JSON.stringify(records));
        } catch (error) {
            console.warn("Could not save submission.", error);
        }

        previewDetails.textContent =
            `Question ${selectedQuestion.id}: ${selectedQuestion.title} • ` +
            `${submission.language} • ` +
            `${new Date(submission.submittedAt).toLocaleString()}`;

        previewCode.textContent = code;
        previewPanel.hidden = false;

        feedbackElement.textContent = "Analyzing your code...";
        showMessage(
            "Submission saved. Checking your code...",
            "success"
        );

        previewPanel.scrollIntoView({
            behavior: "smooth",
            block: "start"
        });

        // Send C++ code to the backend and display its feedback.
        const feedback = await analyzeCode(code);
        feedbackElement.textContent = feedback;

        showMessage(
            "Submission saved. Analyzer feedback is ready.",
            "success"
        );
    });

    updateEditorInfo();
});