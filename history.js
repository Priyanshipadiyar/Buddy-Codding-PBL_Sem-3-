const STORAGE_KEY = "buddyCodingHistory";

const historyTable = document.getElementById("historyTable");
const topSearch = document.getElementById("historySearch");
const searchInput = document.getElementById("searchInput");
const languageFilter = document.getElementById("languageFilter");
const statusFilter = document.getElementById("statusFilter");
const emptyState = document.getElementById("emptyState");
const resultCount = document.getElementById("resultCount");
const detailsPanel = document.getElementById("detailsPanel");

const languageNames = { c: "C", cpp: "C++", "C": "C", "C++": "C++" };

const demoHistory = [
  {
    id: "demo-1",
    title: "Find the Largest Element",
    language: "C",
    date: "2026-10-14T10:30:00",
    status: "Reviewed",
    description: "Find the largest number in an array.",
    code: '#include <stdio.h>\\n\\nint main(void) {\\n    int arr[] = {4, 8, 2, 9, 5};\\n    int max = arr[0];\\n    for (int i = 1; i < 5; i++) {\\n        if (arr[i] > max) max = arr[i];\\n    }\\n    printf("%d", max);\\n    return 0;\\n}',
    feedback: "Sample feedback: Check how the initial maximum is selected and how every array element is compared."
  },
  {
    id: "demo-3",
    title: "Reverse a String",
    language: "C++",
    date: "2026-10-12T12:00:00",
    status: "Submitted",
    description: "Reverse the characters in a string.",
    code: '#include <iostream>\\n#include <string>\\nusing namespace std;\\n\\nint main() {\\n    string text = "hello";\\n    // Attempt saved for review\\n    return 0;\\n}',
    feedback: "Awaiting review."
  }
];

function isSupportedLanguage(language) {
  const normalized = String(language || "").toLowerCase();
  return normalized === "c" || normalized === "cpp" || normalized === "c++";
}

function getHistory() {
  try {
    const stored = localStorage.getItem(STORAGE_KEY);
    if (stored !== null) {
      const parsed = JSON.parse(stored);
      return Array.isArray(parsed) ? parsed.filter(item => isSupportedLanguage(item.language)) : [];
    }
    localStorage.setItem(STORAGE_KEY, JSON.stringify(demoHistory));
    return [...demoHistory];
  } catch (error) {
    console.warn("History storage is unavailable:", error);
    return [...demoHistory];
  }
}

function saveHistory(records) {
  try {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(records));
    return true;
  } catch (error) {
    alert("History could not be saved in this browser.");
    return false;
  }
}

function formatDate(value) {
  const date = new Date(value);
  if (Number.isNaN(date.getTime())) return "Unknown date";
  return date.toLocaleString([], {
    day: "2-digit", month: "short", year: "numeric",
    hour: "2-digit", minute: "2-digit"
  });
}

function statusClass(status) {
  if (status === "Reviewed") return "reviewed";
  if (status === "Needs Improvement") return "improvement";
  return "submitted";
}

function renderStats(records) {
  document.getElementById("totalCount").textContent = records.length;
  document.getElementById("reviewedCount").textContent =
    records.filter(item => item.status === "Reviewed").length;
  document.getElementById("improveCount").textContent =
    records.filter(item => item.status === "Needs Improvement").length;
}

function renderHistory() {
  const records = getHistory();
  const query = `${searchInput.value} ${topSearch.value}`.trim().toLowerCase();
  const selectedLanguage = languageFilter.value;
  const selectedStatus = statusFilter.value;

  const filtered = records.filter(item => {
    const searchable = `${item.title || ""} ${item.description || ""}`.toLowerCase();
    const lang = languageNames[item.language] || languageNames[String(item.language || "").toLowerCase()] || "";
    return searchable.includes(query) &&
      (selectedLanguage === "all" || lang === selectedLanguage) &&
      (selectedStatus === "all" || item.status === selectedStatus);
  });

  historyTable.replaceChildren();

  filtered.forEach(item => {
    const row = document.createElement("tr");
    const title = document.createElement("td");
    title.className = "problem-name";
    title.textContent = item.title || "Untitled Problem";

    const language = document.createElement("td");
    language.textContent = languageNames[item.language] || languageNames[String(item.language || "").toLowerCase()] || "Unknown";

    const date = document.createElement("td");
    date.textContent = formatDate(item.date);

    const statusCell = document.createElement("td");
    const badge = document.createElement("span");
    badge.className = `status ${statusClass(item.status)}`;
    badge.textContent = item.status || "Submitted";
    statusCell.appendChild(badge);

    const action = document.createElement("td");
    const view = document.createElement("button");
    view.type = "button";
    view.className = "view-btn";
    view.textContent = "View Details";
    view.addEventListener("click", () => showDetails(item));
    action.appendChild(view);

    row.append(title, language, date, statusCell, action);
    historyTable.appendChild(row);
  });

  emptyState.hidden = filtered.length > 0;
  resultCount.textContent = `Showing ${filtered.length} of ${records.length} submissions`;
  renderStats(records);
}

function showDetails(item) {
  document.getElementById("detailTitle").textContent = item.title || "Untitled Problem";
  document.getElementById("detailMeta").textContent =
    `Language: ${languageNames[item.language] || languageNames[String(item.language || "").toLowerCase()] || "Unknown"} | Date: ${formatDate(item.date)} | Status: ${item.status || "Submitted"}`;
  document.getElementById("detailDescription").textContent =
    item.description || "No description available.";
  document.getElementById("detailCode").textContent =
    item.code || "No code saved for this submission.";
  document.getElementById("detailFeedback").textContent =
    item.feedback || "No feedback is available yet.";
  detailsPanel.hidden = false;
  detailsPanel.scrollIntoView({ behavior: "smooth", block: "start" });
}

searchInput.addEventListener("input", renderHistory);
topSearch.addEventListener("input", renderHistory);
languageFilter.addEventListener("change", renderHistory);
statusFilter.addEventListener("change", renderHistory);

document.getElementById("closeDetails").addEventListener("click", () => {
  detailsPanel.hidden = true;
});

document.getElementById("clearHistory").addEventListener("click", () => {
  const records = getHistory();
  if (!records.length) {
    alert("There is no history to clear.");
    return;
  }
  if (confirm("Delete all submission history saved in this browser?") && saveHistory([])) {
    detailsPanel.hidden = true;
    renderHistory();
  }
});

const mobileMenu = document.getElementById("mobileMenu");
const sidebar = document.getElementById("sidebar");
mobileMenu.addEventListener("click", () => sidebar.classList.toggle("open"));

document.addEventListener("click", event => {
  if (window.innerWidth <= 720 &&
      sidebar.classList.contains("open") &&
      !sidebar.contains(event.target) &&
      !mobileMenu.contains(event.target)) {
    sidebar.classList.remove("open");
  }
});

document.addEventListener("keydown", event => {
  if ((event.ctrlKey || event.metaKey) && event.key.toLowerCase() === "k") {
    event.preventDefault();
    topSearch.focus();
  }
});

window.addEventListener("storage", event => {
  if (event.key === STORAGE_KEY) renderHistory();
});

renderHistory();