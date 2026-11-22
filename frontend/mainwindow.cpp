#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include "authmanager.h"

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QStackedWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QPainter>
#include <QColor>
#include <QFont>
#include <QMessageBox>
#include <QSizePolicy>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QComboBox>
#include <QDateTime>
#include <QprogressBar>

// Custom colorful icons drawn using C++
class IconBadge : public QWidget
{
public:
    enum Type { Code, Submissions, Solved, Progress, Home, Problems, History, Clock };

    IconBadge(Type type, QWidget *parent = nullptr)
        : QWidget(parent), iconType(type)
    {
        setFixedSize(64, 64);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        QColor bg("#2478f5");

        if (iconType == Solved)
            bg = QColor("#16a889");
        else if (iconType == Progress)
            bg = QColor("#6554f0");
        else if (iconType == History || iconType == Clock)
            bg = QColor("#0b2349");

        p.setPen(Qt::NoPen);
        p.setBrush(bg);
        p.drawEllipse(rect().adjusted(1, 1, -1, -1));

        QPen pen(Qt::white, 3.5, Qt::SolidLine,
                 Qt::RoundCap, Qt::RoundJoin);

        p.setPen(pen);
        p.setBrush(Qt::NoBrush);

        const int cx = width() / 2;
        const int cy = height() / 2;

        switch (iconType)
        {
        case Code:
        case Problems:
            p.drawLine(cx - 5, cy - 12, cx - 15, cy);
            p.drawLine(cx - 15, cy, cx - 5, cy + 12);
            p.drawLine(cx + 5, cy - 12, cx + 15, cy);
            p.drawLine(cx + 15, cy, cx + 5, cy + 12);
            p.drawLine(cx + 3, cy - 15, cx - 3, cy + 15);
            break;

        case Submissions:
            p.setPen(Qt::NoPen);
            p.setBrush(Qt::white);
            p.drawRoundedRect(cx - 12, cy - 15, 24, 30, 3, 3);
            p.setPen(QPen(bg, 2.5));
            p.drawLine(cx - 6, cy - 7, cx + 6, cy - 7);
            p.drawLine(cx - 6, cy, cx + 6, cy);
            p.drawLine(cx - 6, cy + 7, cx + 3, cy + 7);
            break;

        case Solved:
            p.setPen(QPen(Qt::white, 5, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(cx - 11, cy, cx - 3, cy + 9);
            p.drawLine(cx - 3, cy + 9, cx + 13, cy - 10);
            break;

        case Progress:
            p.drawLine(cx - 17, cy + 11, cx - 7, cy + 1);
            p.drawLine(cx - 7, cy + 1, cx, cy + 6);
            p.drawLine(cx, cy + 6, cx + 14, cy - 10);
            p.drawLine(cx + 5, cy - 10, cx + 14, cy - 10);
            p.drawLine(cx + 14, cy - 10, cx + 14, cy - 1);
            break;

        case Home:
            p.setPen(Qt::NoPen);
            p.setBrush(Qt::white);
            {
                QPolygon roof;
                roof << QPoint(cx - 16, cy - 2)
                     << QPoint(cx, cy - 17)
                     << QPoint(cx + 16, cy - 2);

                p.drawPolygon(roof);
                p.drawRect(cx - 12, cy - 2, 24, 17);

                p.setBrush(bg);
                p.drawRect(cx - 3, cy + 5, 6, 10);
            }
            break;

        case History:
        case Clock:
            p.drawEllipse(cx - 13, cy - 13, 26, 26);
            p.drawLine(cx, cy - 7, cx, cy);
            p.drawLine(cx, cy, cx + 8, cy + 5);

            if (iconType == History) {
                p.drawArc(cx - 18, cy - 18, 36, 36,
                          45 * 16, 270 * 16);
                p.drawLine(cx - 17, cy - 14, cx - 17, cy - 4);
                p.drawLine(cx - 17, cy - 14, cx - 7, cy - 14);
            }
            break;
        }
    }

private:
    Type iconType;
};


MainWindow::MainWindow(QString userName, QString userEmail,
                       QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      userName(userName),
      userEmail(userEmail)
{
    ui->setupUi(this);

    // Load logged-in user's separate data
    AuthManager auth;
    QJsonObject userData;
    auth.loadUserData(userEmail, &userData);

    QJsonArray submissions =
        userData.value("submissions").toArray();

    // Calculate user statistics
    int totalSubmissions = submissions.size();
    int solvedCount = 0;
    int improvementCount = 0;

    for (const QJsonValue &value : submissions)
    {
        QJsonObject record = value.toObject();

        QString status =
            record.value("status").toString();

        if (status.compare("Solved", Qt::CaseInsensitive) == 0)
            solvedCount++;

        else if (status.compare("Failed", Qt::CaseInsensitive) == 0 ||
                 status.compare("Needs Improvement",
                                Qt::CaseInsensitive) == 0)
            improvementCount++;
    }

    setWindowTitle("Buddy Coding - C++ Learning Platform");
    resize(1536, 1024);
    setMinimumSize(1000, 700);

    setStyleSheet(R"(
        QMainWindow {
            background-color: #061633;
        }
        QWidget {
            color: #f4f7ff;
            font-family: "Segoe UI";
            font-size: 14px;
        }
        QLabel {
            background: transparent;
            border: none;
        }
        QPushButton {
            background-color: transparent;
            color: #a9c8f8;
            border: none;
            border-radius: 9px;
            padding: 12px;
            text-align: left;
        }
        QPushButton:hover {
            background-color: #102e60;
            color: white;
        }
        QPushButton:checked {
            background-color: #12448f;
            color: white;
        }
        QTableWidget {
            background-color: #071a38;
            alternate-background-color: #0a2042;
            color: #edf4ff;
            border: none;
            gridline-color: #142f56;
            selection-background-color: #173f78;
            outline: none;
        }
        QTableWidget::item {
            padding: 10px;
            border-bottom: 1px solid #142f56;
        }
        QHeaderView::section {
            background-color: #10294f;
            color: #a9c9fa;
            padding: 15px 10px;
            border: none;
            font-weight: bold;
        }
        QScrollBar:vertical {
            background: #061633;
            width: 9px;
        }
        QScrollBar::handle:vertical {
            background: #254a7e;
            border-radius: 4px;
            min-height: 30px;
        }
    )");

    QWidget *central = new QWidget;
    setCentralWidget(central);

    QHBoxLayout *root = new QHBoxLayout(central);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // ---------------- SIDEBAR ----------------

    QFrame *sidebar = new QFrame;
    sidebar->setFixedWidth(310);
    sidebar->setStyleSheet(
        "QFrame { background-color: #04132c;"
        "border-right: 1px solid #18345c; }");

    QVBoxLayout *side = new QVBoxLayout(sidebar);
    side->setContentsMargins(17, 27, 17, 20);
    side->setSpacing(12);

    QHBoxLayout *brandLayout = new QHBoxLayout;
    brandLayout->setSpacing(13);

    IconBadge *brandIcon = new IconBadge(IconBadge::Code);
    brandIcon->setFixedSize(58, 58);
    brandLayout->addWidget(brandIcon);

    QVBoxLayout *brandText = new QVBoxLayout;
    brandText->setSpacing(2);

    QLabel *brand = new QLabel("Buddy Coding");
    brand->setStyleSheet(
        "font-size: 26px; font-weight: bold; color: white;");

    QLabel *tagline = new QLabel("Learn  •  Practice  •  Grow");
    tagline->setStyleSheet("color: #88aeea; font-size: 13px;");

    brandText->addWidget(brand);
    brandText->addWidget(tagline);
    brandLayout->addLayout(brandText);
    side->addLayout(brandLayout);
    side->addSpacing(28);

    auto navButton = [](const QString &text) {
        QPushButton *b = new QPushButton(text);
        b->setCheckable(true);
        b->setMinimumHeight(62);
        b->setCursor(Qt::PointingHandCursor);
        b->setStyleSheet(
            "QPushButton { font-size: 19px; padding-left: 22px; }"
            "QPushButton:checked { background-color: #12448f;"
            "color: white; font-weight: bold; }"
            "QPushButton:hover { background-color: #102e60; }");
        return b;
    };

    QPushButton *homeBtn = navButton("  ⌂    Home");
    QPushButton *problemsBtn = navButton("  </>   Problems");
    QPushButton *historyBtn = navButton("  ◷    History");

    side->addWidget(homeBtn);
    side->addWidget(problemsBtn);
    side->addWidget(historyBtn);
    side->addStretch();

    QLabel *footer = new QLabel("©  Powered by C++ & Qt");
    footer->setStyleSheet("color: #7d9ed0; padding: 10px;");
    side->addWidget(footer);


    // ---------------- PAGES ----------------

    QStackedWidget *pages = new QStackedWidget;

    auto makeTitle = [](const QString &small,
                        const QString &title,
                        const QString &subtitle) {
        QWidget *header = new QWidget;

        QVBoxLayout *layout = new QVBoxLayout(header);
        layout->setContentsMargins(0, 0, 0, 8);
        layout->setSpacing(8);

        QLabel *eyebrow = new QLabel(small.toUpper());
        eyebrow->setStyleSheet(
            "color: #64c8ff; font-size: 12px;"
            "font-weight: bold; letter-spacing: 2px;");

        QLabel *heading = new QLabel(title);
        heading->setWordWrap(true);
        heading->setStyleSheet(
            "font-size: 38px; font-weight: bold; color: white;");

        QLabel *description = new QLabel(subtitle);
        description->setWordWrap(true);
        description->setStyleSheet(
            "font-size: 16px; color: #a7c3ef;");

        layout->addWidget(eyebrow);
        layout->addWidget(heading);
        layout->addWidget(description);

        return header;
    };

    auto makeCard = [](const QString &title,
                       const QString &value,
                       const QString &subtitle,
                       IconBadge::Type type,
                       const QString &circleColor) {

        QFrame *card = new QFrame;
        card->setMinimumHeight(166);
        card->setSizePolicy(
            QSizePolicy::Expanding, QSizePolicy::Preferred);

        card->setStyleSheet(
            "QFrame { background-color: #081b39;"
            "border: 1px solid #1b3a65; border-radius: 13px; }");

        QHBoxLayout *layout = new QHBoxLayout(card);
        layout->setContentsMargins(22, 18, 16, 18);
        layout->setSpacing(20);

        IconBadge *icon = new IconBadge(type);
        icon->setStyleSheet("background: transparent;");
        layout->addWidget(icon, 0, Qt::AlignTop);

        QVBoxLayout *texts = new QVBoxLayout;
        texts->setSpacing(8);

        QLabel *name = new QLabel(title);
        name->setStyleSheet(
            "color: #a8c7f8; font-size: 17px; border: none;");

        QLabel *number = new QLabel(value);
        number->setStyleSheet(
            "color: white; font-size: 38px; font-weight: bold;"
            "border: none;");

        QLabel *desc = new QLabel(subtitle);
        desc->setWordWrap(true);
        desc->setStyleSheet(
            "color: #a5c1ed; font-size: 14px; border: none;");

        texts->addWidget(name);
        texts->addWidget(number);
        texts->addWidget(desc);
        texts->addStretch();

        layout->addLayout(texts, 1);
        Q_UNUSED(circleColor);

        return card;
    };

    auto makeTable = []() {
        QTableWidget *table = new QTableWidget;

        table->setAlternatingRowColors(false);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->verticalHeader()->setVisible(false);
        table->horizontalHeader()->setSectionResizeMode(
            QHeaderView::Stretch);
        table->horizontalHeader()->setMinimumHeight(52);
        table->verticalHeader()->setDefaultSectionSize(64);
        table->setShowGrid(false);
        table->setFocusPolicy(Qt::NoFocus);

        return table;
    };


    // ---------------- DYNAMIC ATTEMPTS TABLE ----------------


    auto fillAttempts = [this, userEmail](QTableWidget *table) {

        AuthManager auth;
        QJsonObject userData;

        auth.loadUserData(userEmail, &userData);

        QJsonArray submissions =
            userData.value("submissions").toArray();

        table->clearContents();
        table->setColumnCount(5);

        table->setHorizontalHeaderLabels(
            {"PROBLEM NAME", "LANGUAGE", "STATUS", "DATE", "ACTION"});

        table->setRowCount(submissions.size());

        for (int row = 0; row < submissions.size(); ++row)
        {
            QJsonObject record =
                submissions.at(row).toObject();

            QString problem =
                record.value("problem").toString();

            QString language =
                record.value("language").toString("C++");

            QString status =
                record.value("status").toString();

            QString date =
                record.value("date").toString();

            QString values[4] = {
                problem, language, status, date
            };

            for (int col = 0; col < 4; ++col)
            {
                QTableWidgetItem *item =
                    new QTableWidgetItem(values[col]);

                item->setTextAlignment(
                    col == 2
                        ? Qt::AlignCenter
                        : Qt::AlignVCenter | Qt::AlignLeft);

                if (col == 2)
                {
                    bool solved =
                        status.compare(
                            "Solved", Qt::CaseInsensitive) == 0;

                    item->setForeground(
                        solved
                            ? QColor("#66f2c5")
                            : QColor("#ff929b"));

                    item->setBackground(
                        solved
                            ? QColor("#10483f")
                            : QColor("#54242f"));

                    QFont f = item->font();
                    f.setBold(true);
                    item->setFont(f);
                }

                table->setItem(row, col, item);
            }

            QPushButton *view = new QPushButton("View");
            view->setFixedSize(78, 38);
            view->setCursor(Qt::PointingHandCursor);

            view->setStyleSheet(
                "QPushButton { background-color: #1257bf;"
                "color: white; border: none; border-radius: 8px;"
                "font-size: 15px; font-weight: bold;"
                "text-align: center; padding: 0px; }"
                "QPushButton:hover { background-color: #2478f5; }");

            QWidget *holder = new QWidget;
            QHBoxLayout *btnLayout = new QHBoxLayout(holder);

            btnLayout->setContentsMargins(5, 0, 5, 0);
            btnLayout->setAlignment(Qt::AlignCenter);
            btnLayout->addWidget(view);

            table->setCellWidget(row, 4, holder);

            QString details =
                "Problem: " + problem +
                "\nLanguage: " + language +
                "\nStatus: " + status +
                "\nDate: " + date +
                "\n\nCode:\n" + record.value("code").toString();

            connect(view, &QPushButton::clicked,
                    this, [this, details]() {

                        QMessageBox::information(
                            this, "Submission Details", details);
                    });
        }
    };


    // ---------------- HOME PAGE ----------------

    QWidget *homePage = new QWidget;

    QVBoxLayout *home = new QVBoxLayout(homePage);
    home->setContentsMargins(38, 34, 38, 28);
    home->setSpacing(24);

    QHBoxLayout *welcomeRow = new QHBoxLayout;

    welcomeRow->addWidget(makeTitle(
        "YOUR PERSONAL CODING GUIDE",
        "Welcome Back,\n" + userName + "!",
        "Keep coding, keep learning, and build something amazing!"));

    QVBoxLayout *quoteLayout = new QVBoxLayout;
    quoteLayout->setAlignment(Qt::AlignTop | Qt::AlignRight);


    // ---------------- LOGOUT BUTTON ----------------

    QPushButton *logoutBtn = new QPushButton("↪  Logout");
    logoutBtn->setFixedSize(130, 44);
    logoutBtn->setCursor(Qt::PointingHandCursor);

    logoutBtn->setStyleSheet(
        "QPushButton {"
        "background-color: #b83245;"
        "color: white;"
        "border: none;"
        "border-radius: 9px;"
        "font-size: 16px;"
        "font-weight: bold;"
        "text-align: center;"
        "}"
        "QPushButton:hover {"
        "background-color: #e04458;"
        "}");

    quoteLayout->addWidget(logoutBtn, 0, Qt::AlignRight);

    connect(logoutBtn, &QPushButton::clicked,
            this, [this]() {

        LoginWindow *login = new LoginWindow;
        login->setAttribute(Qt::WA_DeleteOnClose);
        login->show();
        this->close();
    });

    QLabel *quote = new QLabel(
        "“Small steps\nmake big progress.”");

    quote->setAlignment(Qt::AlignCenter);
    quote->setStyleSheet(
        "color: #c0d7ff; font-size: 18px; font-style: italic;");

    quoteLayout->addWidget(quote);

    QLabel *quoteLine = new QLabel("━━━━");
    quoteLine->setAlignment(Qt::AlignCenter);
    quoteLine->setStyleSheet("color: #2478f5;");

    quoteLayout->addWidget(quoteLine);

    welcomeRow->addLayout(quoteLayout);
    home->addLayout(welcomeRow);


    // ---------------- USER STATISTICS ----------------

    QHBoxLayout *stats = new QHBoxLayout;
    stats->setSpacing(24);

    stats->addWidget(makeCard(
        "Total Submissions",
        QString::number(totalSubmissions),
        "Your coding attempts",
        IconBadge::Submissions, "#2478f5"));

    stats->addWidget(makeCard(
        "Solved",
        QString::number(solvedCount),
        "Problems solved",
        IconBadge::Solved, "#16a889"));

    stats->addWidget(makeCard(
        "Needs Improvement",
        QString::number(improvementCount),
        "Keep practicing",
        IconBadge::Progress, "#6554f0"));

    home->addLayout(stats);


    // ---------------- PREVIOUS ATTEMPTS ----------------


    QFrame *attemptPanel = new QFrame;

    attemptPanel->setStyleSheet(
        "QFrame { background-color: #071a38;"
        "border: 1px solid #17365f; border-radius: 13px; }");

    QVBoxLayout *attemptLayout = new QVBoxLayout(attemptPanel);

    attemptLayout->setContentsMargins(22, 18, 22, 20);
    attemptLayout->setSpacing(17);

    QHBoxLayout *attemptHeading = new QHBoxLayout;

    IconBadge *clockIcon = new IconBadge(IconBadge::Clock);
    clockIcon->setFixedSize(42, 42);

    attemptHeading->addWidget(clockIcon);

    QLabel *attemptTitle =
        new QLabel("Previous Attempts");

    attemptTitle->setStyleSheet(
        "font-size: 27px; font-weight: bold; color: white;");

    attemptHeading->addWidget(attemptTitle);
    attemptHeading->addStretch();

    attemptLayout->addLayout(attemptHeading);


    // Main horizontal layout
    QHBoxLayout *attemptContent = new QHBoxLayout;
    attemptContent->setSpacing(20);


    // -------- LEFT: PREVIOUS ATTEMPTS TABLE --------

    // -------- LEFT: PREVIOUS ATTEMPTS TABLE --------

    QTableWidget *homeTable = makeTable();

    AuthManager dashboardAuth;
    QJsonObject dashboardData;

    dashboardAuth.loadUserData(userEmail, &dashboardData);

    QJsonArray dashboardSubmissions =
        dashboardData.value("submissions").toArray();

    homeTable->setColumnCount(3);

    homeTable->setHorizontalHeaderLabels(
        {"PROBLEM NAME", "LANGUAGE", "STATUS"});

    homeTable->setColumnWidth(0, 350);
    homeTable->setColumnWidth(1, 150);
    homeTable->setColumnWidth(2, 150);

    homeTable->horizontalHeader()->setStretchLastSection(true);

    int totalRows = qMin(5, dashboardSubmissions.size());

    homeTable->setRowCount(totalRows);

    for (int row = 0; row < totalRows; row++)
    {
        QJsonObject record =
            dashboardSubmissions.at(
                                    dashboardSubmissions.size() - 1 - row).toObject();

        QString values[3] = {
            record.value("problem").toString(),
            record.value("language").toString("C++"),
            record.value("status").toString()
        };

        for (int col = 0; col < 3; col++)
        {
            QTableWidgetItem *item =
                new QTableWidgetItem(values[col]);

            item->setTextAlignment(
                (col == 1 || col == 2)
                    ? Qt::AlignCenter
                    : Qt::AlignVCenter | Qt::AlignLeft);

            if (col == 2)
            {
                bool solved =
                    values[col].compare(
                        "Solved", Qt::CaseInsensitive) == 0;

                item->setForeground(
                    solved ? QColor("#66f2c5")
                           : QColor("#ff929b"));
            }

            homeTable->setItem(row, col, item);
        }
    }

    attemptContent->addWidget(homeTable, 3);


    // -------- RIGHT: OVERALL STATUS --------

    QFrame *statusPanel = new QFrame;

    statusPanel->setMinimumWidth(220);

    statusPanel->setStyleSheet(
        "QFrame { background-color: #0b2450;"
        "border: 1px solid #173f78; border-radius: 12px; }");

    QVBoxLayout *statusLayout = new QVBoxLayout(statusPanel);

    QLabel *statusTitle = new QLabel("Overall Status");

    statusTitle->setStyleSheet(
        "font-size: 21px; font-weight: bold; color: white;");

    statusTitle->setAlignment(Qt::AlignCenter);

    statusLayout->addWidget(statusTitle);


    // Calculate status
    int solved = 0;
    int failed = 0;

    for (const QJsonValue &value : dashboardSubmissions)
    {
        QString status =
            value.toObject().value("status").toString();

        if (status.compare("Solved", Qt::CaseInsensitive) == 0)
            solved++;

        else if (status.compare("Failed", Qt::CaseInsensitive) == 0 ||
                 status.compare("Needs Improvement",
                                Qt::CaseInsensitive) == 0)
            failed++;
    }

    int checked = solved + failed;

    int successRate =
        checked > 0 ? (solved * 100 / checked) : 0;


    // Percentage
    QLabel *percentageLabel =
        new QLabel(QString::number(successRate) + "%");

    percentageLabel->setAlignment(Qt::AlignCenter);

    percentageLabel->setStyleSheet(
        "font-size: 34px; font-weight: bold; color: white;");

    statusLayout->addWidget(percentageLabel);


    // Progress diagram
    QProgressBar *statusBar = new QProgressBar;

    statusBar->setRange(0, 100);
    statusBar->setValue(successRate);
    statusBar->setTextVisible(false);
    statusBar->setFixedHeight(17);

    statusBar->setStyleSheet(
        "QProgressBar { background: #54242f;"
        "border: none; border-radius: 8px; }"
        "QProgressBar::chunk { background: #00d6a0;"
        "border-radius: 8px; }");

    statusLayout->addWidget(statusBar);


    // Solved count
    QLabel *solvedLabel =
        new QLabel("Solved: " + QString::number(solved));

    solvedLabel->setStyleSheet(
        "font-size: 16px; color: #66f2c5;");

    statusLayout->addWidget(solvedLabel);


    // Failed count
    QLabel *failedLabel =
        new QLabel("Failed: " + QString::number(failed));

    failedLabel->setStyleSheet(
        "font-size: 16px; color: #ff929b;");

    statusLayout->addWidget(failedLabel);

    statusLayout->addStretch();

    attemptContent->addWidget(statusPanel, 1);

    attemptLayout->addLayout(attemptContent);

    home->addWidget(attemptPanel);
    home->addStretch();



    // ---------------- PROBLEMS PAGE ----------------

    QWidget *problemsPage = new QWidget;

    QVBoxLayout *problemsLayout =
        new QVBoxLayout(problemsPage);

    problemsLayout->setContentsMargins(22, 22, 22, 22);
    problemsLayout->setSpacing(15);

    // Page heading
    QLabel *pageTitle = new QLabel("Coding Practice");

    pageTitle->setStyleSheet(
        "font-size: 32px; font-weight: bold; color: white;");

    QLabel *pageSubtitle = new QLabel(
        "Select a question, write your code and get helpful hints.");

    pageSubtitle->setStyleSheet(
        "font-size: 15px; color: #a7c3ef;");

    problemsLayout->addWidget(pageTitle);
    problemsLayout->addWidget(pageSubtitle);

    // Three column workspace
    QHBoxLayout *workspace = new QHBoxLayout;
    workspace->setSpacing(15);


    // ---------------- QUESTION DATA ----------------

    QStringList questions = {
        "Q01 - Largest Element in an Array",
        "Q02 - Remove Duplicates from Sorted Array",
        "Q03 - Remove Element",
        "Q04 - Merge Sorted Array",
        "Q05 - Best Time to Buy and Sell Stock",
        "Q06 - Maximum Product of Three Numbers",
        "Q07 - Intersection of Two Linked Lists",
        "Q08 - Merge Two Sorted Lists",
        "Q09 - Reverse Linked List",
        "Q10 - Factorial of a Number",
        "Q11 - Check Palindrome String",
        "Q12 - Sum of Digits",
        "Q13 - Fibonacci Series",
        "Q14 - Reverse an Array",
        "Q15 - Linear Search",
        "Q16 - Count Vowels and Consonants",
        "Q17 - Find Minimum Element",
        "Q18 - Check Prime Number",
        "Q19 - Matrix Addition",
        "Q20 - Print Pattern (Pyramid)",
        "Q21 - Count Even and Odd",
        "Q22 - String Length without strlen",
        "Q23 - Swap Two Numbers",
        "Q24 - Leap Year",
        "Q25 - Binary Search",
        "Q26 - Valid Parentheses",
        "Q27 - Middle of Linked List",
        "Q28 - Detect Cycle in Linked List",
        "Q29 - Evaluate Postfix Expression",
        "Q30 - Implement Queue using Stacks",
        "Q31 - Inorder Traversal",
        "Q32 - Max Depth of Binary Tree",
        "Q33 - Bubble Sort",
        "Q34 - Quick Sort Partition",
        "Q35 - Two Sum"
    };

    QStringList descriptions = {
        "Given an integer array, find and return its largest element.",
        "Remove duplicate elements from a sorted array and return the number of unique elements.",
        "Remove all occurrences of a given value from an array.",
        "Merge two sorted arrays into one sorted array.",
        "Given stock prices, find the maximum profit possible by buying and selling once.",
        "Find the maximum product that can be obtained by multiplying any three numbers.",
        "Find the node where two singly linked lists intersect.",
        "Merge two sorted linked lists into one sorted linked list.",
        "Reverse a singly linked list.",
        "Calculate the factorial of a given non-negative integer.",
        "Check whether a given string is a palindrome.",
        "Calculate the sum of all digits of an integer.",
        "Generate the first N terms of the Fibonacci series.",
        "Reverse the elements of an integer array.",
        "Find the position of a target element using linear search.",
        "Count the number of vowels and consonants in a given string.",
        "Find and display the smallest element in an integer array.",
        "Determine whether a given number is prime.",
        "Add two matrices of the same dimensions.",
        "Print a pyramid pattern using stars.",
        "Count the even and odd elements in an integer array.",
        "Find the length of a string without using strlen().",
        "Swap two numbers using a suitable method.",
        "Check whether a given year is a leap year.",
        "Search for a target element in a sorted array using binary search.",
        "Determine whether an expression contains valid and balanced parentheses.",
        "Find the middle node of a singly linked list.",
        "Detect whether a linked list contains a cycle.",
        "Evaluate an arithmetic postfix expression using a stack.",
        "Implement a queue using two stacks.",
        "Perform inorder traversal of a binary tree.",
        "Find the maximum depth or height of a binary tree.",
        "Sort an integer array using bubble sort.",
        "Perform the partition operation used in quick sort.",
        "Find two array elements whose sum equals a given target."
    };

    QString starterCode =
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main()\n"
        "{\n"
        "    // Write your code here\n\n"
        "    return 0;\n"
        "}";

    // ---------------- LEFT PANEL ----------------

    QFrame *questionPanel = new QFrame;
    questionPanel->setFixedWidth(290);

    questionPanel->setStyleSheet(
        "QFrame { background-color: #081b39;"
        "border: 1px solid #1b3a65; border-radius: 12px; }");

    QVBoxLayout *questionLayout =
        new QVBoxLayout(questionPanel);

    QLabel *questionHeading =
        new QLabel("1. Select a Question");

    questionHeading->setStyleSheet(
        "font-size: 20px; font-weight: bold;"
        "color: white; padding: 8px;");

    questionLayout->addWidget(questionHeading);

    QListWidget *questionList = new QListWidget;

    for (int i = 0; i < questions.size(); i++)
    {
        QString level = (i == 9 || i == 11 || i == 12 ||
                         i == 13 || i == 14)
                            ? "Intermediate"
                            : "Beginner";

        questionList->addItem(
            QString::number(i + 1) + ". " +
            questions[i] + "   [" + level + "]");
    }

    questionList->setStyleSheet(R"(
    QListWidget {
        background-color: #071a38;
        color: white;
        border: none;
        font-size: 14px;
    }
    QListWidget::item {
        padding: 12px;
        border-bottom: 1px solid #142f56;
    }
    QListWidget::item:selected {
        background-color: #1257bf;
        color: white;
        border-radius: 6px;
    }
)");

    questionLayout->addWidget(questionList);


    // ---------------- CENTER PANEL ----------------

    QFrame *editorPanel = new QFrame;

    editorPanel->setStyleSheet(
        "QFrame { background-color: #081b39;"
        "border: 1px solid #1b3a65; border-radius: 12px; }");

    QVBoxLayout *editorLayout =
        new QVBoxLayout(editorPanel);

    QLabel *editorHeading =
        new QLabel("2. Enter Your Code");

    editorHeading->setStyleSheet(
        "font-size: 20px; font-weight: bold;"
        "color: white; padding: 8px;");

    editorLayout->addWidget(editorHeading);

    QLabel *selectedQuestion =
        new QLabel(questions[0]);

    selectedQuestion->setStyleSheet(
        "font-size: 19px; font-weight: bold; color: #64c8ff;");

    editorLayout->addWidget(selectedQuestion);

    QLabel *description =
        new QLabel(descriptions[0]);

    description->setWordWrap(true);
    description->setStyleSheet(
        "font-size: 14px; color: #a7c3ef;");

    editorLayout->addWidget(description);

    QComboBox *languageBox = new QComboBox;
    languageBox->addItems({"C", "C++"});
    languageBox->setCurrentText("C++");

    languageBox->setStyleSheet(
        "background-color: #10294f; color: white;"
        "padding: 8px; border-radius: 6px;");

    editorLayout->addWidget(languageBox);

    QPlainTextEdit *codeEditor = new QPlainTextEdit;

    codeEditor->setPlainText(starterCode);

    codeEditor->setStyleSheet(R"(
    QPlainTextEdit {
        background-color: #061327;
        color: white;
        border: 1px solid #254a7e;
        border-radius: 8px;
        padding: 10px;
        font-family: Consolas;
        font-size: 15px;
    }
)");

    editorLayout->addWidget(codeEditor, 1);
    // Language-wise starter code

    connect(languageBox, &QComboBox::currentTextChanged,
            this, [codeEditor](const QString &language)
            {
                if (language == "C")
                {
                    codeEditor->setPlainText(
                        "#include <stdio.h>\n\n"
                        "int main()\n"
                        "{\n"
                        "    // Write your code here\n\n"
                        "    return 0;\n"
                        "}");
                }
                else
                {
                    codeEditor->setPlainText(
                        "#include <iostream>\n"
                        "using namespace std;\n\n"
                        "int main()\n"
                        "{\n"
                        "    // Write your code here\n\n"
                        "    return 0;\n"
                        "}");
                }
            });

    QPushButton *submitBtn =
        new QPushButton("Submit Code");

    submitBtn->setMinimumHeight(48);

    submitBtn->setStyleSheet(
        "QPushButton { background-color: #1261e8;"
        "color: white; font-size: 16px; font-weight: bold;"
        "border-radius: 8px; text-align: center; }"
        "QPushButton:hover { background-color: #2478f5; }");

    editorLayout->addWidget(submitBtn);


    // ---------------- RIGHT PANEL ----------------

    QFrame *hintPanel = new QFrame;
    hintPanel->setFixedWidth(265);

    hintPanel->setStyleSheet(
        "QFrame { background-color: #081b39;"
        "border: 1px solid #1b3a65; border-radius: 12px; }");

    QVBoxLayout *hintLayout =
        new QVBoxLayout(hintPanel);

    QLabel *hintHeading =
        new QLabel("3. Hint Generation");

    hintHeading->setStyleSheet(
        "font-size: 20px; font-weight: bold;"
        "color: white; padding: 8px;");

    hintLayout->addWidget(hintHeading);

    QLabel *hintIcon = new QLabel("💡");
    hintIcon->setAlignment(Qt::AlignCenter);
    hintIcon->setStyleSheet("font-size: 55px;");

    hintLayout->addWidget(hintIcon);

    QLabel *hintNumber = new QLabel("Hint 0 / 7");
    hintNumber->setAlignment(Qt::AlignCenter);
    hintNumber->setStyleSheet(
        "color: #64c8ff; font-size: 17px; font-weight: bold;");

    hintLayout->addWidget(hintNumber);

    QLabel *hintText = new QLabel(
        "Select a question to receive helpful guidance.");

    hintText->setWordWrap(true);
    hintText->setAlignment(Qt::AlignCenter);
    hintText->setStyleSheet(
        "background-color: #10294f; color: white;"
        "padding: 18px; border-radius: 9px; font-size: 15px;");

    hintLayout->addWidget(hintText);

    QPushButton *nextHintBtn =
        new QPushButton("Reveal Next Hint");

    nextHintBtn->setMinimumHeight(48);

    nextHintBtn->setStyleSheet(
        "QPushButton { background-color: #102f68;"
        "color: white; border: 1px solid #1261e8;"
        "border-radius: 8px; font-weight: bold;"
        "text-align: center; }"
        "QPushButton:hover { background-color: #1257bf; }");

    hintLayout->addWidget(nextHintBtn);
    hintLayout->addStretch();


    // ---------------- ADD PANELS ----------------

    workspace->addWidget(questionPanel, 3);
    workspace->addWidget(editorPanel, 5);
    workspace->addWidget(hintPanel, 3);

    problemsLayout->addLayout(workspace, 1);


    // ---------------- QUESTION SELECTION ----------------

    QStringList hints = {
        "Understand the input and expected output first.",
        "Identify what information you need to store in variables.",
        "Break the problem into smaller logical steps.",
        "Decide which loop or condition is required.",
        "Think about the starting and ending conditions.",
        "Check how variables should be updated in each step.",
        "Trace your logic with a small example before submitting."
    };

    // Store current hint number safely
    hintNumber->setProperty("currentHint", 0);

    connect(questionList, &QListWidget::currentRowChanged,
            this, [=](int index)
            {
                if (index < 0 || index >= questions.size())
                    return;

                selectedQuestion->setText(questions[index]);
                description->setText(descriptions[index]);

                codeEditor->setPlainText(starterCode);

                hintNumber->setProperty("currentHint", 0);

                hintNumber->setText("Hint 0 / 7");
                hintText->setText(
                    "Select Reveal Next Hint to see guidance.");

                nextHintBtn->setText("Reveal Next Hint");
                nextHintBtn->setEnabled(true);
            });


    // ---------------- REVEAL NEXT HINT ----------------

    connect(nextHintBtn, &QPushButton::clicked,
            this, [=]()
            {
                int currentHint =
                    hintNumber->property("currentHint").toInt();

                if (currentHint < hints.size())
                {
                    hintText->setText(hints[currentHint]);

                    currentHint++;

                    hintNumber->setText(
                        "Hint " + QString::number(currentHint) + " / 7");

                    hintNumber->setProperty(
                        "currentHint", currentHint);
                }

                if (currentHint == 7)
                {
                    nextHintBtn->setText("All Hints Revealed");
                    nextHintBtn->setEnabled(false);
                }
            });

    // ---------------- SUBMIST CODE ----------------


    connect(submitBtn, &QPushButton::clicked,
            this, [this, codeEditor, selectedQuestion, languageBox , userEmail]() {

                QString code = codeEditor->toPlainText().trimmed();

                if (code.isEmpty())
                {
                    QMessageBox::warning(
                        this, "Empty Code",
                        "Please write your code before submitting.");
                    return;
                }

                AuthManager auth;
                QJsonObject userData;

                if (!auth.loadUserData(userEmail, &userData))
                {
                    QMessageBox::warning(
                        this, "Error",
                        "Unable to load user data.");
                    return;
                }

                QJsonArray submissions =
                    userData.value("submissions").toArray();

                QJsonObject record;

                record["problem"] = selectedQuestion->text();
                record["language"] = languageBox->currentText();
                record["code"] = code;
                record["status"] = "Submitted";
                record["date"] =
                    QDateTime::currentDateTime().toString(
                        "dd-MM-yyyy hh:mm AP");

                submissions.append(record);
                userData["submissions"] = submissions;

                QString error;

                if (!auth.saveUserData(userEmail, userData, &error))
                {
                    QMessageBox::warning(
                        this, "Save Error", error);
                    return;
                }

                QMessageBox::information(
                    this, "Submission Saved",
                    "Your code has been saved successfully!\n\n"
                    "Problem: " + selectedQuestion->text() +
                        "\nLanguage: " + languageBox->currentText());
            });


    // Select first question by default
    questionList->setCurrentRow(0);


    // ---------------- HISTORY PAGE ----------------

    QWidget *historyPage = new QWidget;

    QVBoxLayout *historyLayout =
        new QVBoxLayout(historyPage);

    historyLayout->setContentsMargins(38, 34, 38, 28);
    historyLayout->setSpacing(22);

    historyLayout->addWidget(makeTitle(
        "YOUR LEARNING JOURNEY",
        "Submission History",
        "Review your previous coding attempts and their status."));

    QTableWidget *historyTable = makeTable();

    historyLayout->addWidget(historyTable);
    historyLayout->addStretch();


    // ---------------- NAVIGATION ----------------

    pages->addWidget(homePage);
    pages->addWidget(problemsPage);
    pages->addWidget(historyPage);

    auto showPage =
        [pages, homeBtn, problemsBtn, historyBtn](int index) {

            pages->setCurrentIndex(index);

            homeBtn->setChecked(index == 0);
            problemsBtn->setChecked(index == 1);
            historyBtn->setChecked(index == 2);
        };

    connect(homeBtn, &QPushButton::clicked,
            this, [showPage]() {
                showPage(0);
            });

    connect(problemsBtn, &QPushButton::clicked,
            this, [showPage]() {
                showPage(1);
            });

    connect(historyBtn, &QPushButton::clicked,
            this, [showPage, fillAttempts, historyTable]() {

                fillAttempts(historyTable);
                showPage(2);
            });

    root->addWidget(sidebar);
    root->addWidget(pages, 1);

    showPage(0);
}


MainWindow::~MainWindow()
{
    delete ui;
}