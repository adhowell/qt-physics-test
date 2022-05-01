#include "items/line.h"
#include <QFrame>
#include <QGraphicsView>

class View;

class GraphicsView : public QGraphicsView {
Q_OBJECT
public:
    GraphicsView(View *v) : QGraphicsView(), mView(v) { mLine = new Line(); }
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void timerEvent(QTimerEvent* event) override;

    void collisionCalc();
    Line* getLine() { return mLine; }

signals:
    void singleClick(qreal x, qreal y);

private:
    View *mView;
    Line* mLine;
};

class View : public QFrame {
public:
    explicit View(QWidget *parent = nullptr);

    QGraphicsView* getQGraphicsView() const;
    GraphicsView* getGraphicsView() const;

private:
    GraphicsView *mGraphicsView;
};
