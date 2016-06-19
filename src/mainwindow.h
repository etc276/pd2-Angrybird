#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QDesktopWidget>
#include <QTimer>
#include <Box2D/Box2D.h>
#include <QMouseEvent>
#include <iostream>
#include <QMessageBox>
#include <QPushButton>

#include <gameitem.h>
#include <land.h>
#include <bird.h>
#include <block.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showEvent(QShowEvent *);
    bool eventFilter(QObject *,QEvent *event);
    void closeEvent(QCloseEvent *);
signals:
    // Signal for closing the game
    void quitGame();

private slots:
    void tick();
    // For debug slot
    void QUITSLOT();
    void boundaryCheck();
    void on_Reset_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    b2World *world;
    QList<GameItem *> itemList;
    QTimer timer;
    Bird *birdR, *birdY, *birdG, *birdB, *pig1, *pig2;
    Block *block1, *block2, *block3, *block4;
    float xBegin, xEnd, yBegin, yEnd, numBird;
    bool flying, flag, end;
    int score;
    QPushButton *Reset, *Quit;
};

#endif // MAINWINDOW_H
