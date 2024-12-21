#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class MenuWindow : public QWidget {
    Q_OBJECT
public:
    explicit MenuWindow(QWidget *parent = nullptr);

signals:
    void startGame(int mode);

private slots:
    void startGameJvJ();
    void startGameJvIA();
};

#endif // MENUWINDOW_H
