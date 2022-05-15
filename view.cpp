#include "view.h"
#include "items/ball.h"
#include "items/wall.h"
#include "collision_stuff/vector.h"
#include <QtWidgets>
#include <QGraphicsView>
#include <QFrame>
#include <QDebug>

qreal GraphicsView::sMaxTemp = 1.0;

GraphicsView::GraphicsView(View *v) : QGraphicsView(), mView(v), mUniformRng(0, 1) {
    mLine = new Line();
}

void GraphicsView::addEnergyLabel()
{
    mEnergyText = scene()->addText("");
    mEnergyText->setPos(-3, -20);
}

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

    if (event->modifiers() == Qt::KeyboardModifier::ShiftModifier)
    {
        auto ball = new Ball(QColor(0, 0, 0),
                             mapToScene(event->x(), event->y()),
                             Vector(0, 0),
                             0.1,
                             2);
        scene()->addItem(ball);
    }

    if (mLine->getVisibility()) {
        QPointF eventPos = mapToScene(event->x(), event->y());
        mLine->updateEnd(eventPos);
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (mLine->getVisibility()) {
        mLine->setVisibility(false);
        qreal mass = mUniformRng(mRng) * (Ball::sMaxMass - Ball::sMinMass) + Ball::sMinMass;
        qreal radius = mUniformRng(mRng) * (Ball::sMaxRadius - Ball::sMinRadius) + Ball::sMinRadius;
        auto ball = new Ball(QColor(0, 0, 0),
                             mLine->getStartPos(),
                             Vector(mLine->getLine(), mLine->getLine().length()*0.01),
                             mass,
                             radius);
        scene()->addItem(ball);
    }
}

void GraphicsView::timerEvent(QTimerEvent *event)
{
    mLine->update();
    if (mAccuracy) {
        accuratePhysicsCalc();
    } else {
        fastPhysicsCalc();
    }
    deleteOutOfBounds();
}

void GraphicsView::fastPhysicsCalc() {
    qreal dx = mGravityStrength * (mDeltaT / mSubSamples) * qCos(mGravityDirection);
    qreal dy = mGravityStrength * (mDeltaT / mSubSamples) * qSin(mGravityDirection);
    Vector gravity = Vector(dx, dy);

    QVector<Ball *> balls_lmao;
    QVector<Wall *> walls;
    const QList<QGraphicsItem *> items = scene()->items();
    for (QGraphicsItem *item: items) {
        if (Ball *ball = qgraphicsitem_cast<Ball *>(item)) {
            balls_lmao << ball;
            ball->setAccelerationVector(gravity);
        }
        if (Wall *wall = qgraphicsitem_cast<Wall *>(item)) {
            wall->update(); // Update wall colour
            walls << wall;
        }
    }

    qreal subSampleDeltaT = mDeltaT / mSubSamples;

    // Sub-sample the current frame
    for (int s = 0; s <= mSubSamples; s++) {

        // Check for balls going to the wall(s)
        for (auto b: balls_lmao) {
            for (auto w: walls) {
                qreal dist = b->getRadius() - b->distance(w);
                if (dist >= 0) {
                    b->posUpdate(dist + 0.1, w->getNormVec().atan2());
                    b->vectorReflect(w->getNormVec().atan2());

                    // Approximate inelastic collision
                    if (mInelastic) b->velocityMultiply(0.9);
                    b->velocityAddition(mTemp * sMaxTemp);
                }
            }

            for (auto otherB : balls_lmao) {
                if (b == otherB) continue;

                qreal dist = b->getRadius() + otherB->getRadius() - b->distance(otherB);
                if (dist >= 0) {
                    qreal atan2 = b->atan2(otherB);
                    b->posUpdate(dist * 0.5 + 0.1, atan2);
                    otherB->posUpdate(dist * 0.5 + 0.1, atan2 + M_PI);
                    b->collide(otherB);

                    // Approximate inelastic collision
                    if (mInelastic) {
                        b->velocityMultiply(0.9);
                        otherB->velocityMultiply(0.9);
                    }
                }
            }

            // Update positions by sub time-step
            b->advance(subSampleDeltaT);
        }
    }
    // Draw calls and get the current energy total
    qreal energy = 0;
    for (auto b: balls_lmao) {
        b->update();
        energy += b->getEnergy();
    }
    energy /= 1000.0;
    mEnergyText->setPlainText(QString("%1 kJ").arg(energy, 0, 'f', 5));
}

void GraphicsView::accuratePhysicsCalc()
{
    qreal dx = mGravityStrength * qCos(mGravityDirection);
    qreal dy = mGravityStrength * qSin(mGravityDirection);
    Vector gravity = Vector(dx, dy);

    QVector<Ball*> balls_lmao;
    QVector<Wall*> walls;
    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Ball *ball = qgraphicsitem_cast<Ball*>(item)) {
            ball->setAccelerationVector(gravity);
            balls_lmao << ball;
        }
        if (Wall *wall = qgraphicsitem_cast<Wall*>(item)) {
            wall->update(); // Update wall colour
            walls << wall;
        }
    }

    qreal subSampleDeltaT = mDeltaT / mSubSamples;

    // Sub-sample the current time-step
    for (int s = 0; s <= mSubSamples; s++) {

        for (auto b : balls_lmao)
            b->setTimeRemaining(subSampleDeltaT);

        for (int c = 0; c <= mCollisionTreeLimit; c++) {

            for (auto b: balls_lmao) {

                if (b->getTimeRemaining() > 0.0) {
                    b->storePosition();

                    // Update positions by sub time-step
                    b->advance(b->getTimeRemaining());
                }
            }

            for (auto b: balls_lmao) {

                if (b->getTimeRemaining() <= 0.0) continue;

                // Check for balls going to the wall(s)
                for (auto w: walls) {
                    qreal dist = b->getRadius() - b->distance(w);
                    if (dist >= 0) {
                        b->posUpdate(dist + 0.1, w->getNormVec().atan2());
                        b->vectorReflect(w->getNormVec().atan2());

                        // Approximate inelastic collision
                        if (mInelastic) b->velocityMultiply(0.9);
                        b->velocityAddition(mTemp * sMaxTemp);
                    }
                }

                // Then check for collision with other balls
                for (auto otherB: balls_lmao) {
                    if (b == otherB) continue;
                    qreal dist = b->getRadius() + otherB->getRadius() - b->distance(otherB);
                    if (dist >= 0) {
                        qreal atan2 = b->atan2(otherB);
                        b->posUpdate(dist * 0.5 + 0.1, atan2);
                        otherB->posUpdate(dist * 0.5 + 0.1, atan2 + M_PI);
                        b->collide(otherB);

                        // Approximate inelastic collision
                        if (mInelastic) {
                            b->velocityMultiply(0.9);
                            otherB->velocityMultiply(0.9);
                        }
                    }
                }

                qreal actualDeltaT = b->getDeltaPos() / b->getVelocity();
                b->decrementTime(actualDeltaT);
            }
        }
    }

    // Draw calls and get the current energy total
    qreal energy = 0;
    for (auto b: balls_lmao) {
        b->update();
        energy += b->getEnergy();
    }
    energy /= 1000.0;
    mEnergyText->setPlainText(QString("%1 kJ").arg(energy, 0, 'f', 5));
}

View::View(QWidget* parent) : QFrame(parent)
{
    setFrameStyle(Sunken | StyledPanel);
    mGraphicsView = new GraphicsView(this);
    mGraphicsView->ensureVisible(QRectF(0, 0, 0, 0));

    mTempSlider = new QSlider();
    mTempSlider->setMinimum(0);
    mTempSlider->setMaximum(1000);
    mTempSlider->setValue(0);
    mTempSlider->setOrientation(Qt::Vertical);
    mTempSlider->setToolTip("Wall temperature");

    mGravitySlider = new QSlider();
    mGravitySlider->setMinimum(0);
    mGravitySlider->setMaximum(1000);
    mGravitySlider->setValue(0);
    mGravitySlider->setOrientation(Qt::Vertical);
    mGravitySlider->setToolTip("Gravity strength");

    mGravityDial = new QDial();
    mGravityDial->setWrapping(true);
    mGravityDial->setToolTip("Gravity direction");

    mDeleteAllButton = new QToolButton();
    mDeleteAllButton->setText("Clear screen");
    mDeleteAllButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mCollisionModeButton = new QToolButton();
    mCollisionModeButton->setText("Inelastic physics");
    mCollisionModeButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mCollisionModeButton->setCheckable(true);

    mAccuracyButton = new QToolButton();
    mAccuracyButton->setText("Accurate physics");
    mAccuracyButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    mAccuracyButton->setCheckable(true);

    QToolBar* toolbar = new QToolBar;
    toolbar->addWidget(mCollisionModeButton);
    toolbar->addWidget(mAccuracyButton);
    toolbar->addWidget(mDeleteAllButton);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    toolbar->addSeparator();
    toolbar->addWidget(mTempSlider);
    toolbar->addSeparator();

    spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    toolbar->addSeparator();
    toolbar->addWidget(mGravitySlider);
    toolbar->addSeparator();
    toolbar->addWidget(mGravityDial);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(mGraphicsView, 0, 0);
    layout->addWidget(toolbar, 1, 0);
    setLayout(layout);

    connect(mDeleteAllButton, &QToolButton::pressed,
            mGraphicsView, &GraphicsView::clearItems);
    connect(mGravityDial, &QDial::sliderMoved,
            mGraphicsView, &GraphicsView::adjustGravityDirection);
    connect(mGravityDial, &QDial::sliderPressed,
            this, &View::setGravityDirection);
    connect(mGravitySlider, &QSlider::sliderMoved,
            mGraphicsView, &GraphicsView::adjustGravityStrength);
    connect(mTempSlider, &QSlider::sliderMoved,
            mGraphicsView, &GraphicsView::adjustTemperature);
    connect(mCollisionModeButton, &QToolButton::toggled,
            mGraphicsView, &GraphicsView::toggleInelastic);
    connect(mAccuracyButton, &QToolButton::toggled,
            mGraphicsView, &GraphicsView::toggleAccuracy);
}

void View::setGravityDirection()
{
    mGraphicsView->adjustGravityDirection(mGravityDial->value());
}

void GraphicsView::deleteOutOfBounds()
{
    QVector<Ball*> balls;
    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Ball *ball = qgraphicsitem_cast<Ball*>(item)) {
            QPointF pos = ball->getPos();
            if (pos.x() < 0 || pos.x() > gWidth - gBuffer || pos.y() < 0 || pos.y() > gHeight - gBuffer)
                balls << ball;
        }
    }
    for (auto b : balls) {
        scene()->removeItem(b);
        delete b;
    }
}

void GraphicsView::clearItems()
{
    QVector<Ball*> balls;
    QVector<Wall*> walls;
    const QList<QGraphicsItem*> items = scene()->items();
    for (QGraphicsItem *item : items) {
        if (Ball *ball = qgraphicsitem_cast<Ball*>(item))
            balls << ball;
        if (Wall *wall = qgraphicsitem_cast<Wall*>(item)) {
            if (wall->deletable()) walls << wall;
        }
    }
    for (auto b : balls) {
        scene()->removeItem(b);
        delete b;
    }
    for (auto w : walls) {
        scene()->removeItem(w);
        delete w;
    }
}

void GraphicsView::adjustGravityDirection(int d)
{
    mGravityDirection = 2.0 * M_PI * qreal(d)/100.0 + M_PI_2;
}

void GraphicsView::adjustGravityStrength(int d)
{
    mGravityStrength = mMaxGravityStrength * qreal(d)/1000.0;
}

void GraphicsView::adjustTemperature(int d)
{
    mTemp = qreal(d)/1000.0;
}

void GraphicsView::toggleInelastic()
{
    mInelastic = !mInelastic;
}

void GraphicsView::toggleAccuracy()
{
    mAccuracy = !mAccuracy;
}


QGraphicsView* View::getQGraphicsView() const
{
    return static_cast<QGraphicsView *>(mGraphicsView);
}

GraphicsView* View::getGraphicsView() const
{
    return mGraphicsView;
}