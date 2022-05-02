#include "mainwindow.h"
#include "collision_stuff/box.h"
#include "items/ball.h"
#include "items/wall.h"
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(QRectF(0, 0,gWidth-gBuffer, gHeight-gBuffer));

    mView = new View();
    mView->getQGraphicsView()->setScene(mScene);

    connect(mView->getGraphicsView(), &GraphicsView::singleClick,
            this, &MainWindow::createBox);

    auto layout = new QVBoxLayout;
    layout->addWidget(mView);
    setLayout(layout);

    setWindowTitle("Interactive collision detection thing");
    setFixedSize(gWidth, gHeight+100);

    mScene->addItem(mView->getGraphicsView()->getLine());
    createWall(QLineF(QPointF(0, 0), QPointF(gWidth-100, 0)), false);
    createWall(QLineF(QPointF(gWidth-100, 0), QPointF(gWidth-100, gHeight-100)), false);
    createWall(QLineF(QPointF(gWidth-100, gHeight-100), QPointF(0, gHeight-100)), false);
    createWall(QLineF(QPointF(0, gHeight-100), QPointF(0, 0)), false);

    mView->getGraphicsView()->addEnergyLabel();

    mView->getGraphicsView()->startTimer(1000/60);
};

void MainWindow::createBox(qreal x, qreal y)
{
    qreal* temp = mView->getGraphicsView()->getTemp();
    auto box = new Box(x, y, temp);
    for (auto w : box->getWalls())
        mScene->addItem(w);
}

void MainWindow::createWall(QLineF l, bool canBeDeleted)
{
    qreal* temp = mView->getGraphicsView()->getTemp();
    auto item = new Wall(l, temp, canBeDeleted);
    mScene->addItem(item);
}