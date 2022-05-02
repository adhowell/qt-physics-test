#include "mainwindow.h"
#include "collision_stuff/box.h"
#include "items/ball.h"
#include "items/wall.h"
#include "view.h"
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(QRectF(0, 0,gWidth-gBuffer, gHeight-gBuffer));

    auto view = new View();
    view->getQGraphicsView()->setScene(mScene);

    connect(view->getGraphicsView(), &GraphicsView::singleClick,
            this, &MainWindow::createBox);

    auto layout = new QVBoxLayout;
    layout->addWidget(view);
    setLayout(layout);

    setWindowTitle("Interactive collision detection thing");
    setFixedSize(gWidth, gHeight+100);

    mScene->addItem(view->getGraphicsView()->getLine());
    createWall(QLineF(QPointF(0, 0), QPointF(gWidth-100, 0)), false);
    createWall(QLineF(QPointF(gWidth-100, 0), QPointF(gWidth-100, gHeight-100)), false);
    createWall(QLineF(QPointF(gWidth-100, gHeight-100), QPointF(0, gHeight-100)), false);
    createWall(QLineF(QPointF(0, gHeight-100), QPointF(0, 0)), false);

    view->getGraphicsView()->addEnergyLabel();

    view->getGraphicsView()->startTimer(1000/60);
};

void MainWindow::createBox(qreal x, qreal y)
{
    auto box = new Box(x, y);
    for (auto w : box->getWalls())
        mScene->addItem(w);
}

void MainWindow::createWall(QLineF l, bool canBeDeleted)
{
    auto item = new Wall(QColor(0, 0, 0), l, canBeDeleted);
    mScene->addItem(item);
}