#include "mainwindow.h"
#include "collision_stuff/box.h"
#include "items/ball.h"
#include "items/wall.h"
#include "view.h"
#include "global_config.h"
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    mScene = new QGraphicsScene(this);
    mScene->setSceneRect(QRectF(0, 0,gWidth-gBuffer, gHeight-gBuffer));

    auto view = new View();
    view->getQGraphicsView()->setScene(mScene);

    connect(view->getGraphicsView(), &GraphicsView::singleClick,
            this, &MainWindow::createBox);

    auto layout = new QHBoxLayout;
    layout->addWidget(view);
    setLayout(layout);

    setWindowTitle("Interactive collision detection thing");
    setFixedSize(gWidth, gHeight);

    mScene->addItem(view->getGraphicsView()->getLine());
    createWall(QLineF(QPointF(0, 0), QPointF(gWidth-100, 0)));
    createWall(QLineF(QPointF(gWidth-100, 0), QPointF(gWidth-100, gHeight-100)));
    createWall(QLineF(QPointF(gWidth-100, gHeight-100), QPointF(0, gHeight-100)));
    createWall(QLineF(QPointF(0, gHeight-100), QPointF(0, 0)));

    view->getGraphicsView()->addEnergyLabel();

    view->getGraphicsView()->startTimer(1000/60);
};

void MainWindow::createBox(qreal x, qreal y)
{
    auto box = new Box(x, y);
    for (auto w : box->getWalls())
        mScene->addItem(w);
}

void MainWindow::createWall(QLineF l)
{
    auto item = new Wall(QColor(0, 0, 0), l);
    mScene->addItem(item);
}