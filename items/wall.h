#include "../collision_stuff/vector.h"
#include <QGraphicsItem>

#pragma once

/**
 * Line for balls to collide with
 */
class Wall : public QGraphicsItem {
public:
    explicit Wall(const QColor &color, QLineF l);
    explicit Wall(QLineF l);
    enum { Type = 4 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    QLineF getLine() { return mLine; }

    /**
     * Returns the vector normal to the line, pointing
     * in the direction of line travel + 90 degrees.
     */
    NormVector getNormVec() { return mNormVector; }

private:
    QColor mColor;
    QLineF mLine;
    NormVector mNormVector;
};


