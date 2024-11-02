#pragma once

#include <QtWidgets/QMainWindow>
#include <QtMultimedia/QSoundEffect>
#include <QTimer>
#include <QKeyEvent>
#include "ui_denryuuirairabou.h"

class DenryuuIraIraBou : public QMainWindow
{
    Q_OBJECT

public:
    DenryuuIraIraBou(QWidget *parent = nullptr);
    ~DenryuuIraIraBou();

    void resizeEvent(QResizeEvent* event);
    void keyPressEvent(QKeyEvent* event);
    void paintBoard();
    bool hitCheck();
    bool winCheck();
    void newGame();

public slots:
    void onTimer();
private:
    Ui::DenryuuIraIraBouClass ui;
    QTimer* timer;
    QSoundEffect* s0;
    QSoundEffect* s1;
    QSoundEffect* s2;
    QPixmap* bgImg[8];
    QPoint ballPos = QPoint(16, 206);
    int ballBaseRadius = 16;
    int ballRadius = 16;
    int indexTime = 0;
    int bgIndex = 0;
    int bgBaseWidth = 640;
    int bgBaseHeight = 480;
    int bgWidth = 640;
    int bgHeight = 480;
    double scaleRate = 1.0;
    bool isFinished = false;
    bool winOrLose = false;
    int gameInterval = 8000;
};
