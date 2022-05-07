#include "items/line.h"
#include <QFrame>
#include <QGraphicsView>
#include <QtWidgets>
#include <random>
#include "global_config.h"

#pragma once

class View;

class GraphicsView : public QGraphicsView {
Q_OBJECT
public:
    GraphicsView(View *v);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent* event) override;

    void fastPhysicsCalc();
    void accuratePhysicsCalc();
    void addEnergyLabel();
    Line* getLine() { return mLine; }
    qreal* getTemp() { return &mTemp; }

    static qreal sMaxTemp;

public slots:
    void clearItems();
    void adjustGravityDirection(int i);
    void adjustGravityStrength(int i);
    void adjustTemperature(int i);
    void toggleInelastic();
    void toggleAccuracy();

signals:
    void singleClick(qreal x, qreal y);

private:
    View *mView;
    Line* mLine;
    qreal mTemp = 0.0;
    qreal mDeltaT = 1.0;
    qreal mSubSamples = 3;
    qreal mCollisionTreeLimit = 5;
    qreal mGravityDirection = M_PI_2;
    qreal mGravityStrength = 0.0;
    qreal mMaxGravityStrength = gGravity;
    std::mt19937 mRng;
    std::uniform_real_distribution<double> mUniformRng;
    QGraphicsTextItem* mEnergyText;
    bool mInelastic = false;
    bool mAccuracy = false;
};

class View : public QFrame {
Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);

    QGraphicsView* getQGraphicsView() const;
    GraphicsView* getGraphicsView() const;

public slots:
    void setGravityDirection();

signals:
    void clearScreen();

private:
    GraphicsView* mGraphicsView;
    QDial* mGravityDial;
    QSlider* mGravitySlider;
    QSlider* mTempSlider;
    QToolButton* mCollisionModeButton;
    QToolButton* mAccuracyButton;
    QToolButton* mDeleteAllButton;
};
