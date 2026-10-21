const loginForm = document.getElementById("loginForm");
const passwordInput = document.getElementById("password");
const togglePassword = document.getElementById("togglePassword");
const message = document.getElementById("message");
const forgotPassword = document.getElementById("forgotPassword");

togglePassword.addEventListener("click", () => {
    const isHidden = passwordInput.type === "password";

    passwordInput.type = isHidden ? "text" : "password";
    togglePassword.textContent = isHidden ? "Hide" : "Show";
});

loginForm.addEventListener("submit", (event) => {
    event.preventDefault();

    message.className = "message";
    message.textContent = "";

    if (!loginForm.checkValidity()) {
        loginForm.reportValidity();
        return;
    }

    message.classList.add("success");
    message.textContent =
        "Login page design is working! Actual login requires backend authentication.";
});

forgotPassword.addEventListener("click", (event) => {
    event.preventDefault();

    message.className = "message";
    message.classList.add("error");
    message.textContent =
        "Password recovery will be available after backend setup.";
});