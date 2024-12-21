#include "menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Welcome to", this);
    title->setAlignment(Qt::AlignCenter);
    QFont titleFont = title->font();
    titleFont.setPointSize(24);
    title->setFont(titleFont);

    QLabel *imageLabel = new QLabel(this);
    QPixmap pixmap(":/images/hive_logo_web.png");
    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);

    QPushButton *jvJButton = new QPushButton("Joueur VS Joueur", this);
    jvJButton->setFixedSize(200, 50);
    jvJButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(jvJButton, &QPushButton::clicked, this, &MenuWindow::startGameJvJ);

    QPushButton *jvIAButton = new QPushButton("Joueur VS IA", this);
    jvIAButton->setFixedSize(200, 50);
    jvIAButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(jvIAButton, &QPushButton::clicked, this, &MenuWindow::startGameJvIA);

    layout->addWidget(title);
    layout->addWidget(imageLabel);
    layout->addWidget(jvJButton);
    layout->addWidget(jvIAButton);
    layout->setAlignment(jvJButton, Qt::AlignCenter);
    layout->setAlignment(jvIAButton, Qt::AlignCenter);

    setLayout(layout);
}

void MenuWindow::startGameJvJ() {
    emit startGame(1);
}

void MenuWindow::startGameJvIA() {
    emit startGame(2);
}
