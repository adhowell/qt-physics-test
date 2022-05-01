#include "view.h"
#include "items/ball.h"
#include "items/wall.h"
#include "collision_stuff/vector.h"
#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    QPointF eventPos = mapToScene(event->x(), event->y());
    if (event->modifiers() == Qt::KeyboardModifier::ControlModifier)
        emit singleClick(eventPos.x(), eventPos.y());
    else {
        mLine->setVisibility(true);
        mLine->updateBoth(eventPos);
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if (mLine->getVisibility()) {
        QPointF eventPos = mapToScene(event->x(), event->y());
        mLine->updateEnd(eventPos);
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (mLine->getVisibility()) {
        mLine->setVisibility(false);
        auto ball = new Ball(QColor(0, 0, 0),
                             mLine->getStartPos(),
                             VelocityVector(mLine->getLine(),
                                            mLine->getLine().length()*0.01));
        scene()->addItem(ball);
    }
}

void GraphicsView::timerEvent(QTimerEvent *event)
{
    mLine->update();
    collisionCalc();
}

void GraphicsView::collisionCalc()
{
    QVector<Ball*> balls_lmao;
    QVector<Wall*> walls;
    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Ball *ball = qgraphicsitem_cast<Ball*>(item))
            balls_lmao << ball;
        if (Wall *wall = qgraphicsitem_cast<Wall*>(item))
            walls << wall;
    }

    // Check for balls going to the wall(s)
    for (auto b : balls_lmao) {
        for (auto w : walls) {
            qreal dist = Ball::sRadius - b->distance(w);
            if (dist >= 0) {
                b->posUpdate(dist, w->getNormVec().atan2());

                // Need to do this twice as the wall is fixed
                b->vectorUpdate(w->getNormVec().atan2());
                b->vectorUpdate(w->getNormVec().atan2());
            }
        }
    }

    // Then check for collision with other balls
    int numBalls = balls_lmao.size();
    for (int i; i < numBalls; i++) {
        auto bI = balls_lmao[i];
        for (int j = i+1; j < numBalls; j++) {
            auto bJ = balls_lmao[j];
            qreal dist = Ball::sRadius*2.0 - bI->distance(bJ);
            if (dist >= 0) {
                // Elastic collision, equal mass
                qreal iVel = bI->getVel();
                bI->velUpdate(bJ->getVel());
                bJ->velUpdate(iVel);

                bI->posUpdate(dist*0.5, bI->atan2(bJ));
                bJ->posUpdate(dist*0.5, bJ->atan2(bI));
                bI->vectorUpdate(bI->atan2(bJ));
                bJ->vectorUpdate(bJ->atan2(bI));
            }
        }
    }

    // Update positions by time-step
    for (auto b : balls_lmao) {
        b->advance(1);
    }
}

View::View(QWidget* parent) : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);
    mGraphicsView = new GraphicsView(this);
    mGraphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    QGridLayout* topLayout = new QGridLayout;
    topLayout->addWidget(mGraphicsView, 0, 0);
    setLayout(topLayout);
}

QGraphicsView* View::getQGraphicsView() const
{
    return static_cast<QGraphicsView *>(mGraphicsView);
}

GraphicsView* View::getGraphicsView() const
{
    return mGraphicsView;
}