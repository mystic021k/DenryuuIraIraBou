#include <QBitmap>
#include "denryuuirairabou.h"

DenryuuIraIraBou::DenryuuIraIraBou(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    timer->setInterval(0);
    s0 = new QSoundEffect();
    s0->setSource(QUrl::fromLocalFile(":/sound/wav/s0.wav"));
    s0->setLoopCount(QSoundEffect::Infinite);
    s1 = new QSoundEffect();
    s1->setSource(QUrl::fromLocalFile(":/sound/wav/s1.wav"));
    s1->setLoopCount(1);
    s2 = new QSoundEffect();
    s2->setSource(QUrl::fromLocalFile(":/sound/wav/s2.wav"));
    s2->setLoopCount(QSoundEffect::Infinite);
    newGame();
}

DenryuuIraIraBou::~DenryuuIraIraBou()
{}

void DenryuuIraIraBou::resizeEvent(QResizeEvent* event)
{
    ui.graphicsView->setGeometry(0, 0, this->width(), this->height());
    double scaleRateW = (double)(this->width()) / (double)bgBaseWidth;
    double scaleRateH = (double)(this->height()) / (double)bgBaseHeight;
    scaleRate = scaleRateW < scaleRateH ? scaleRateW : scaleRateH;
    scaleRate = floor(scaleRate * 10) / 10;
    double xPosRate = (double)(ballPos.x()) / (double)bgWidth;
    double yPosRate = (double)(ballPos.y()) / (double)bgHeight;
    bgWidth = bgBaseWidth * scaleRate;
    bgHeight = bgBaseHeight * scaleRate;
    ballRadius = ballBaseRadius * scaleRate;
    ballPos.setX((double)bgWidth * xPosRate);
    ballPos.setY((double)bgHeight * yPosRate);
    if (ballPos.x() < 0 + ballRadius)
    {
        ballPos.setX(ballRadius);
    }
    if (ballPos.y() < 0 + ballRadius)
    {
        ballPos.setY(ballRadius);
    }
    if (ballPos.x() > bgWidth - ballRadius)
    {
        ballPos.setX(bgWidth - ballRadius);
    }
    if (ballPos.y() > bgHeight - ballRadius)
    {
        ballPos.setY(bgHeight - ballRadius);
    }
    paintBoard();
}

void DenryuuIraIraBou::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F1)
    {
        newGame();
    }
    if (event->key() == Qt::Key_F2)
    {
        ballBaseRadius = 16;
        newGame();
    }
    if (event->key() == Qt::Key_F3)
    {
        ballBaseRadius = 8;
        newGame();
    }
    if (event->key() == Qt::Key_F5)
    {
        gameInterval = 4000;
        newGame();
    }
    if (event->key() == Qt::Key_F6)
    {
        gameInterval = 8000;
        newGame();
    }
    if (event->key() == Qt::Key_F7)
    {
        gameInterval = 16000;
        newGame();
    }
    if (event->key() == Qt::Key_F8)
    {
        gameInterval = 32000;
        newGame();
    }
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
    if (isFinished)
    {
        return;
    }
    bool arrowPressed = false;
    if (event->modifiers() == Qt::ShiftModifier) {
        if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
        {
            ballPos.setX(ballPos.x() + 1);
            arrowPressed = true;
        }
        if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
        {
            ballPos.setX(ballPos.x() - 1);
            arrowPressed = true;
        }
        if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
        {
            ballPos.setY(ballPos.y() + 1);
            arrowPressed = true;
        }
        if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
        {
            ballPos.setY(ballPos.y() - 1);
            arrowPressed = true;
        }
    }
    else
    {
        if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
        {
            ballPos.setX(ballPos.x() + 8 * scaleRate);
            arrowPressed = true;
        }
        if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
        {
            ballPos.setX(ballPos.x() - 8 * scaleRate);
            arrowPressed = true;
        }
        if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
        {
            ballPos.setY(ballPos.y() + 8 * scaleRate);
            arrowPressed = true;
        }
        if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
        {
            ballPos.setY(ballPos.y() - 8 * scaleRate);
            arrowPressed = true;
        }
    }
    if (ballPos.x() < 0 + ballRadius)
    {
        ballPos.setX(ballRadius);
    }
    if (ballPos.y() < 0 + ballRadius)
    {
        ballPos.setY(ballRadius);
    }
    if (ballPos.x() > bgWidth - ballRadius)
    {
        ballPos.setX(bgWidth - ballRadius);
    }
    if (ballPos.y() > bgHeight - ballRadius)
    {
        ballPos.setY(bgHeight - ballRadius);
    }
    if (arrowPressed)
    {
        if (hitCheck())
        {
            s1->play();
            isFinished = true;
            winOrLose = false;
            timer->stop();
        }
        if (winCheck())
        {
            s0->stop();
            s2->play();
            isFinished = true;
            winOrLose = true;
            timer->stop();
        }
        paintBoard();
    }
}

void DenryuuIraIraBou::paintBoard()
{
    QGraphicsScene* scene = new QGraphicsScene(this);
    QPixmap p(":/image/png/bgbmp" + QString("%1").arg(bgIndex) + "_1.png");
    p = p.scaled(bgWidth, bgHeight);
    scene->addPixmap(p);
    QPen ballPen;
    ballPen.setStyle(Qt::NoPen);
    QRadialGradient radialGrad(QPoint(ballPos.x() - ballRadius / 4, ballPos.y() - ballRadius / 4), ballRadius);
    radialGrad.setColorAt(0, Qt::white);
    radialGrad.setColorAt(1, Qt::yellow);
    QBrush ballBrush(radialGrad);
    scene->addEllipse(ballPos.x() - ballRadius, ballPos.y() - ballRadius, ballRadius * 2, ballRadius * 2, ballPen, ballBrush);
    if (isFinished)
    {
        if (winOrLose)
        {
            QPixmap endImg(":/image/png/win_1.png");
            endImg = endImg.scaled(bgWidth, bgHeight);
            endImg.setMask(endImg.createMaskFromColor(QColor(0, 0, 0)));
            scene->addPixmap(endImg);
        }
        else
        {
            QPixmap endImg(":/image/png/lose_1.png");
            endImg = endImg.scaled(bgWidth, bgHeight);
            endImg.setMask(endImg.createMaskFromColor(QColor(0, 0, 0)));
            scene->addPixmap(endImg);
        }
    }
    ui.graphicsView->setScene(scene);
}

bool DenryuuIraIraBou::hitCheck()
{
    double a2 = atan(1) / 2;
    double a3 = atan(1);
    double a4 = atan(1) / 2 * 3;
    double x[16], y[16];
    x[0] = (double)ballRadius;
    y[0] = 0;
    x[1] = ((double)ballRadius) * cos(a2);
    y[1] = ((double)ballRadius) * sin(a2);
    x[2] = ((double)ballRadius) * cos(a3);
    y[2] = ((double)ballRadius) * sin(a3);
    x[3] = ((double)ballRadius) * cos(a4);
    y[3] = ((double)ballRadius) * sin(a4);
    x[4] = 0;
    y[4] = (double)ballRadius;
    x[5] = -((double)ballRadius) * cos(a4);
    y[5] = ((double)ballRadius) * sin(a4);
    x[6] = -((double)ballRadius) * cos(a3);
    y[6] = ((double)ballRadius) * sin(a3);
    x[7] = -((double)ballRadius) * cos(a2);
    y[7] = ((double)ballRadius) * sin(a2);
    x[8] = -(double)ballRadius;
    y[8] = 0;
    x[9] = -((double)ballRadius) * cos(a2);
    y[9] = -((double)ballRadius) * sin(a2);
    x[10] = -((double)ballRadius) * cos(a3);
    y[10] = -((double)ballRadius) * sin(a3);
    x[11] = -((double)ballRadius) * cos(a4);
    y[11] = -((double)ballRadius) * sin(a4);
    x[12] = 0;
    y[12] = -(double)ballRadius;
    x[13] = ((double)ballRadius) * cos(a4);
    y[13] = -((double)ballRadius) * sin(a4);
    x[14] = ((double)ballRadius) * cos(a3);
    y[14] = -((double)ballRadius) * sin(a3);
    x[15] = ((double)ballRadius) * cos(a2);
    y[15] = -((double)ballRadius) * sin(a2);
    QPoint hitPoints[16] = {
        QPoint(ballPos.x() + qRound(x[0]), ballPos.y() + qRound(y[0])),
        QPoint(ballPos.x() + qRound(x[1]), ballPos.y() + qRound(y[1])),
        QPoint(ballPos.x() + qRound(x[2]), ballPos.y() + qRound(y[2])),
        QPoint(ballPos.x() + qRound(x[3]), ballPos.y() + qRound(y[3])),
        QPoint(ballPos.x() + qRound(x[4]), ballPos.y() + qRound(y[4])),
        QPoint(ballPos.x() + qRound(x[5]), ballPos.y() + qRound(y[5])),
        QPoint(ballPos.x() + qRound(x[6]), ballPos.y() + qRound(y[6])),
        QPoint(ballPos.x() + qRound(x[7]), ballPos.y() + qRound(y[7])),
        QPoint(ballPos.x() + qRound(x[8]), ballPos.y() + qRound(y[8])),
        QPoint(ballPos.x() + qRound(x[9]), ballPos.y() + qRound(y[9])),
        QPoint(ballPos.x() + qRound(x[10]), ballPos.y() + qRound(y[10])),
        QPoint(ballPos.x() + qRound(x[11]), ballPos.y() + qRound(y[11])),
        QPoint(ballPos.x() + qRound(x[12]), ballPos.y() + qRound(y[12])),
        QPoint(ballPos.x() + qRound(x[13]), ballPos.y() + qRound(y[13])),
        QPoint(ballPos.x() + qRound(x[14]), ballPos.y() + qRound(y[14])),
        QPoint(ballPos.x() + qRound(x[15]), ballPos.y() + qRound(y[15]))
    };
    QImage m(":/image/png/mbgbmp" + QString("%1").arg(bgIndex) + "_1.png");
    m = m.scaled(bgWidth, bgHeight);
    for (int i = 0; i < 16; i++)
    {
        if (m.pixelColor(hitPoints[i]) == QColor(0, 0, 0))
        {
            return true;
        }
    }
    return false;
}

bool DenryuuIraIraBou::winCheck()
{
    if (ballPos.x() > bgWidth - ballRadius * 2)
    {
        return true;
    }
    return false;
}

void DenryuuIraIraBou::newGame()
{
    ballPos = QPoint(ballBaseRadius * scaleRate, 206 * scaleRate);
    isFinished = false;
    winOrLose = false;
    timer->start();
    s1->stop();
    s2->stop();
    s0->play();
    resizeEvent(nullptr);
}

void DenryuuIraIraBou::onTimer()
{
    if (isFinished)
    {
        return;
    }
    indexTime++;
    if (indexTime % gameInterval == 0)
    {
        indexTime = 0;
        bgIndex++;
        bgIndex = bgIndex % 8;
        paintBoard();
    }
}
