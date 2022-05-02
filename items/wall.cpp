#include "wall.h"
#include <QPainter>

Wall::Wall(const QColor &color, QLineF l, bool canBeDeleted) : mNormVector(-l.dy(), l.dx())
{
    mColor = color;
    mLine = l;
    mDeletable = canBeDeleted;
}

Wall::Wall(QLineF l, bool canBeDeleted) : mColor(QColor(0, 0, 0)), mNormVector(-l.dy(), l.dx())
{
    mLine = l;
    mDeletable = canBeDeleted;
}

QRectF Wall::boundingRect() const {
    QPointF p1 = mLine.p1();
    QPointF p2 = mLine.p2();
    return QRectF(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
                  std::abs(p1.x() - p2.x()), std::abs(p1.y() - p2.y()));
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QPen pen;
    pen.setColor(mColor);
    painter->setPen(pen);
    painter->drawLine(mLine);
}