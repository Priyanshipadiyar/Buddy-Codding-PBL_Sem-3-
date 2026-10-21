const form = document.getElementById("chatForm");
const userInput = document.getElementById("userInput");
const conversation = document.getElementById("conversation");
const welcome = document.getElementById("welcome");
const hintButton = document.getElementById("hintButton");
const hintLevelLabel = document.getElementById("hintLevel");
const fileInput = document.getElementById("fileInput");
const historySearch = document.getElementById("historySearch");
const sidebar = document.getElementById("sidebar");
const mobileMenu = document.getElementById("mobileMenu");

let hintLevel = 1;

function addMessage(text, sender) {
  const message = document.createElement("div");
  message.className = `message ${sender}`;

  const label = document.createElement("span");
  label.className = "message-label";
  label.textContent = sender === "user" ? "You" : "Buddy Coding";

  const content = document.createElement("div");
  content.textContent = text;

  message.append(label, content);
  conversation.appendChild(message);
  conversation.scrollTop = conversation.scrollHeight;
}

function getGuidance(text, mode = "hint") {
  const value = text.toLowerCase();

  if (mode === "spot") {
    return "Let's inspect your logic. Check loop conditions, variable updates, array boundaries and edge cases. Trace one small input and compare expected output with actual output.";
  }

  if (mode === "question") {
    return "Guiding question: what should happen in the first step? Which condition tells your algorithm when to stop? Try tracing one small example.";
  }

  if (/linked.?list|node|pointer|reverse list/.test(value)) {
    return hintLevel === 1
      ? "Level 1 hint: draw three nodes and mark their next pointers. What information should you save before changing a pointer?"
      : "Level 2 hint: save the next node before changing the current node's next pointer. Then move your previous and current references forward.";
  }

  if (/two sum|hash.?map|target|pair sum/.test(value)) {
    return hintLevel === 1
      ? "Level 1 hint: for each number, find the complementary value needed to reach the target. How can you check whether it was already seen?"
      : "Level 2 hint: calculate target - currentValue, search for that complement in a hash map, then store the current value and index.";
  }

  if (/tree|recursion|traversal/.test(value)) {
    return hintLevel === 1
      ? "Level 1 hint: identify the base case. Is the current node processed before, between, or after its children?"
      : "Level 2 hint: handle a null node as the base case, then visit left and right children in the required traversal order.";
  }

  if (/loop|array|index|off.?by.?one/.test(value)) {
    return hintLevel === 1
      ? "Level 1 hint: trace a small input. Write each index and variable value after every iteration."
      : "Level 2 hint: check whether your loop should use < or <=, and keep array indices between 0 and length - 1.";
  }

  return hintLevel === 1
    ? "Level 1 hint: divide the problem into input, processing and output. Try one small example manually."
    : "Level 2 hint: write the steps in pseudocode, check each condition and test an edge case such as empty or single-item input.";
}

function sendText(text, mode = "hint") {
  const cleaned = text.trim();

  if (!cleaned) return;

  welcome.classList.add("hidden");
  addMessage(cleaned, "user");
  addMessage(getGuidance(cleaned, mode), "buddy");

  userInput.value = "";
  userInput.style.height = "auto";
  userInput.focus();
}

form.addEventListener("submit", (event) => {
  event.preventDefault();
  sendText(userInput.value);
});

hintButton.addEventListener("click", () => {
  hintLevel = hintLevel === 1 ? 2 : 1;
  hintLevelLabel.textContent = hintLevel === 1 ? "Nudge" : "Strong hint";
});

document.getElementById("spotBug").addEventListener("click", () => {
  if (userInput.value.trim()) {
    sendText(userInput.value, "spot");
  } else {
    addMessage("Paste your code or describe the issue in the input box first.", "buddy");
  }
});

document.getElementById("guidingQuestion").addEventListener("click", () => {
  if (userInput.value.trim()) {
    sendText(userInput.value, "question");
  } else {
    addMessage("Enter a problem or code first, then I'll ask a guiding question.", "buddy");
  }
});

fileInput.addEventListener("change", async () => {
  const file = fileInput.files[0];
  if (!file) return;

  try {
    userInput.value = await file.text();
    userInput.focus();
  } catch {
    addMessage("Could not read this file. Please paste its text into the input box.", "buddy");
  }
});

userInput.addEventListener("input", () => {
  userInput.style.height = "auto";
  userInput.style.height = `${Math.min(userInput.scrollHeight, 150)}px`;
});

historySearch.addEventListener("input", () => {
  const query = historySearch.value.toLowerCase().trim();

  document.querySelectorAll(".history-item").forEach((item) => {
    item.classList.toggle(
      "hidden",
      !item.textContent.toLowerCase().includes(query)
    );
  });
});

document.querySelectorAll("[data-prompt]").forEach((button) => {
  button.addEventListener("click", () => {
    userInput.value = button.dataset.prompt;
    userInput.focus();
    sidebar.classList.remove("open");
  });
});

document.querySelectorAll(".nav-item").forEach((button) => {
  button.addEventListener("click", () => {
    document.querySelectorAll(".nav-item").forEach((item) => {
      item.classList.remove("active");
    });

    button.classList.add("active");

    if (button.dataset.view === "problems") {
      userInput.value = "Help me practise arrays, linked lists, stacks, queues and trees.";
      userInput.focus();
    } else if (button.dataset.view === "company") {
      userInput.value = "Help me practise common company interview questions.";
      userInput.focus();
    } else if (button.dataset.view === "history") {
      historySearch.focus();
    } else {
      welcome.classList.remove("hidden");
    }

    sidebar.classList.remove("open");
  });
});

mobileMenu.addEventListener("click", () => {
  sidebar.classList.toggle("open");
});

document.addEventListener("keydown", (event) => {
  if ((event.ctrlKey || event.metaKey) && event.key.toLowerCase() === "k") {
    event.preventDefault();
    historySearch.focus();
  }
});