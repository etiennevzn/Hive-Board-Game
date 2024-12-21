#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "plateau.h"
#include "partie.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void setGameMode(int mode);
    ~MainWindow();
    void initializeGame();  // New method for game initialization

private:
    Plateau* hexBoard;
    Partie* partie;
    Joueur* joueurBlanc;
    Joueur* joueurNoir;
    int gameMode;

private slots:
    void handleGameOver(QString message);
};

#endif // MAINWINDOW_H
