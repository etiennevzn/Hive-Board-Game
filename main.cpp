#include <QApplication>
#include "mainwindow.h"
#include "menuwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MenuWindow menu;
    MainWindow mainWindow;

    QObject::connect(&menu, &MenuWindow::startGame, [&mainWindow, &menu](int mode) {
        mainWindow.setGameMode(mode);
        mainWindow.initializeGame();
        mainWindow.show();
        menu.close();
    });

    menu.show();
    return app.exec();
}
