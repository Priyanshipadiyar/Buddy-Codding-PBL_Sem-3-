#include "loginwindow.h"
#include "mainwindow.h"
#include "signupwindow.h"
#include "authmanager.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Buddy Coding - Login");
    setMinimumSize(950, 600);
    resize(1100, 680);

    setStyleSheet(R"(
        QWidget {
            background-color: #061633;
            color: #f4f7ff;
            font-family: "Segoe UI";
            font-size: 14px;
        }

        QLineEdit {
            background-color: #091d3b;
            border: 1px solid #29466f;
            border-radius: 8px;
            padding: 12px;
            color: white;
        }

        QLineEdit:focus {
            border: 1px solid #3985ff;
        }

        QPushButton {
            background-color: #2478f5;
            color: white;
            border: none;
            border-radius: 8px;
            padding: 13px;
            font-size: 15px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #1763d4;
        }
    )");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(55, 40, 55, 40);
    mainLayout->setSpacing(55);

    // LEFT: Welcome Section
    QWidget *welcomePanel = new QWidget(this);

    QVBoxLayout *welcomeLayout =
        new QVBoxLayout(welcomePanel);

    welcomeLayout->setAlignment(Qt::AlignCenter);
    welcomeLayout->setSpacing(18);

    QLabel *logo = new QLabel("BC");
    logo->setAlignment(Qt::AlignCenter);
    logo->setFixedSize(85, 85);

    logo->setStyleSheet(
        "background-color: #2478f5;"
        "color: white;"
        "border-radius: 20px;"
        "font-size: 30px;"
        "font-weight: bold;");

    QLabel *brand = new QLabel("Buddy Coding");
    brand->setAlignment(Qt::AlignCenter);

    brand->setStyleSheet(
        "font-size: 34px;"
        "font-weight: bold;"
        "color: white;");

    QLabel *tagline =
        new QLabel("Learn. Practice. Grow.");

    tagline->setAlignment(Qt::AlignCenter);

    tagline->setStyleSheet(
        "font-size: 19px;"
        "color: #77b5ff;");

    QLabel *description = new QLabel(
        "Your coding journey starts here.\n"
        "Practice problems and improve your skills.");

    description->setAlignment(Qt::AlignCenter);
    description->setWordWrap(true);

    description->setStyleSheet(
        "font-size: 15px;"
        "color: #a9bfdf;");

    welcomeLayout->addWidget(logo, 0, Qt::AlignHCenter);
    welcomeLayout->addWidget(brand);
    welcomeLayout->addWidget(tagline);
    welcomeLayout->addSpacing(8);
    welcomeLayout->addWidget(description);

    // RIGHT: Login Card
    QFrame *loginCard = new QFrame(this);
    loginCard->setObjectName("loginCard");
    loginCard->setMinimumWidth(350);
    loginCard->setMaximumWidth(420);

    loginCard->setStyleSheet(
        "QFrame#loginCard {"
        "background-color: #0b2041;"
        "border: 1px solid #1d3a63;"
        "border-radius: 16px;"
        "}");

    QVBoxLayout *formLayout =
        new QVBoxLayout(loginCard);

    formLayout->setContentsMargins(30, 32, 30, 32);
    formLayout->setSpacing(15);

    QLabel *title = new QLabel("Welcome Back");

    title->setStyleSheet(
        "font-size: 28px;"
        "font-weight: bold;"
        "background: transparent;"
        "border: none;");

    QLabel *subtitle =
        new QLabel("Sign in to Buddy Coding");

    subtitle->setStyleSheet(
        "color: #a9bfdf;"
        "background: transparent;"
        "border: none;");

    QLabel *emailLabel =
        new QLabel("Email Address");

    emailLabel->setStyleSheet(
        "background: transparent; border: none;");

    QLineEdit *emailInput = new QLineEdit;
    emailInput->setPlaceholderText("Enter your email");
    emailInput->setMinimumHeight(45);

    QLabel *passwordLabel =
        new QLabel("Password");

    passwordLabel->setStyleSheet(
        "background: transparent; border: none;");

    QLineEdit *passwordInput = new QLineEdit;
    passwordInput->setPlaceholderText("Enter your password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(45);

    // Sign In Button
    QPushButton *signInButton =
        new QPushButton("Sign In");

    signInButton->setMinimumHeight(48);
    signInButton->setCursor(Qt::PointingHandCursor);

    // Sign Up Button
    QPushButton *signUpButton =
        new QPushButton("Create New Account");

    signUpButton->setMinimumHeight(45);
    signUpButton->setCursor(Qt::PointingHandCursor);

    signUpButton->setStyleSheet(
        "QPushButton {"
        "background-color: transparent;"
        "color: #77b5ff;"
        "border: 1px solid #29466f;"
        "border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "background-color: #102b50;"
        "}");

    QLabel *status = new QLabel("");
    status->setWordWrap(true);

    status->setStyleSheet(
        "color: #ffcc80;"
        "background: transparent;"
        "border: none;");

    // Add widgets
    formLayout->addWidget(title);
    formLayout->addWidget(subtitle);
    formLayout->addSpacing(12);
    formLayout->addWidget(emailLabel);
    formLayout->addWidget(emailInput);
    formLayout->addWidget(passwordLabel);
    formLayout->addWidget(passwordInput);
    formLayout->addSpacing(8);
    formLayout->addWidget(signInButton);
    formLayout->addWidget(signUpButton);
    formLayout->addWidget(status);

    mainLayout->addWidget(welcomePanel, 1);
    mainLayout->addWidget(loginCard, 1, Qt::AlignVCenter);

    // SIGN IN: Verify account and get user details
    connect(signInButton, &QPushButton::clicked,
            this, [this, emailInput, passwordInput, status]() {

                AuthManager auth;

                QString error;
                QString userName;
                QString userEmail;

                bool success = auth.loginWithUser(
                    emailInput->text(),
                    passwordInput->text(),
                    &userName,
                    &userEmail,
                    &error
                    );

                if (!success) {
                    status->setStyleSheet("color: #ffcc80;");
                    status->setText(error);
                    return;
                }

                // Pass logged-in user's details to Dashboard
                MainWindow *dashboard =
                    new MainWindow(userName, userEmail);

                dashboard->setAttribute(Qt::WA_DeleteOnClose);
                dashboard->show();

                this->close();
            });

    // SIGN UP: Open Signup Page
    connect(signUpButton, &QPushButton::clicked,
            this, [this]() {

                SignupWindow *signup = new SignupWindow;

                signup->setAttribute(Qt::WA_DeleteOnClose);
                signup->show();

                this->close();
            });
}