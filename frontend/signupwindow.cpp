#include "signupwindow.h"
#include "loginwindow.h"
#include "authmanager.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

SignupWindow::SignupWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Buddy Coding - Sign Up");
    resize(500, 650);
    setMinimumSize(450, 600);

    setStyleSheet(R"(
        QWidget {
            background-color: #061633;
            color: white;
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
            padding: 12px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: #1763d4;
        }

        QLabel#subtitle {
            color: #a9bfdf;
        }
    )");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(45, 30, 45, 30);
    layout->setSpacing(14);

    QLabel *logo = new QLabel("Buddy Coding");
    logo->setAlignment(Qt::AlignCenter);
    logo->setStyleSheet(
        "font-size: 30px; font-weight: bold; color: white;");

    QLabel *title = new QLabel("Create Account");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "font-size: 24px; font-weight: bold;");

    QLabel *subtitle = new QLabel("Join Buddy Coding today");
    subtitle->setObjectName("subtitle");
    subtitle->setAlignment(Qt::AlignCenter);

    QLineEdit *nameInput = new QLineEdit;
    nameInput->setPlaceholderText("Full Name");
    nameInput->setMinimumHeight(44);

    QLineEdit *emailInput = new QLineEdit;
    emailInput->setPlaceholderText("Email Address");
    emailInput->setMinimumHeight(44);

    QLineEdit *passwordInput = new QLineEdit;
    passwordInput->setPlaceholderText("Create Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    passwordInput->setMinimumHeight(44);

    QLineEdit *confirmInput = new QLineEdit;
    confirmInput->setPlaceholderText("Confirm Password");
    confirmInput->setEchoMode(QLineEdit::Password);
    confirmInput->setMinimumHeight(44);

    QLabel *message = new QLabel;
    message->setWordWrap(true);
    message->setAlignment(Qt::AlignCenter);
    message->setStyleSheet("color: #ffcc80;");

    QPushButton *signupButton =
        new QPushButton("Create Account");
    signupButton->setMinimumHeight(46);

    QPushButton *loginButton =
        new QPushButton("Already have an account? Sign In");

    loginButton->setStyleSheet(
        "QPushButton { background: transparent; color: #77b5ff; }");

    layout->addWidget(logo);
    layout->addWidget(title);
    layout->addWidget(subtitle);
    layout->addSpacing(8);
    layout->addWidget(nameInput);
    layout->addWidget(emailInput);
    layout->addWidget(passwordInput);
    layout->addWidget(confirmInput);
    layout->addWidget(signupButton);
    layout->addWidget(message);
    layout->addStretch();
    layout->addWidget(loginButton);

    // Signup with AuthManager
    connect(signupButton, &QPushButton::clicked,
            this, [=]() {

                if (passwordInput->text() != confirmInput->text()) {
                    message->setStyleSheet("color: #ffcc80;");
                    message->setText("Passwords do not match.");
                    return;
                }

                AuthManager auth;
                QString error;

                bool success = auth.signup(
                    nameInput->text(),
                    emailInput->text(),
                    passwordInput->text(),
                    &error
                    );

                if (success) {
                    message->setStyleSheet("color: #7ee2a8;");
                    message->setText(
                        "Account created successfully! Please Sign In.");

                    nameInput->clear();
                    emailInput->clear();
                    passwordInput->clear();
                    confirmInput->clear();
                }
                else {
                    message->setStyleSheet("color: #ffcc80;");
                    message->setText(error);
                }
            });

    // Back to Login
    connect(loginButton, &QPushButton::clicked,
            this, [this]() {

                LoginWindow *login = new LoginWindow;
                login->setAttribute(Qt::WA_DeleteOnClose);
                login->show();

                close();
            });
}