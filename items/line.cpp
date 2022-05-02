#include "line.h"
#include "../global_config.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include <QtMath>

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    // Length-dependent colour
    QLineF l = QLineF(mP1, mP2);
    qreal normLen = qMin(l.length()/300.0, 1.0);
    brush.setColor(QColor(int(255*normLen), 0, int(255*(1-normLen))));

    // Create an arrow based on length
    QPolygonF linePoly;
    qreal arrowTipLen = l.length() * 0.33;
    qreal rads = qAtan2(mP2.y()-mP1.y(), mP2.x()-mP1.x());

    qreal outer = 0.2*normLen;
    qreal inner = 0.1*normLen;

    linePoly << mP1;
    linePoly << QPointF(mP1.x() + qCos(rads+outer) * arrowTipLen,
                        mP1.y() + qSin(rads+outer) * arrowTipLen);
    linePoly << QPointF(mP1.x() + qCos(rads+inner) * arrowTipLen,
                        mP1.y() + qSin(rads+inner) * arrowTipLen);
    linePoly << mP2;
    linePoly << QPointF(mP1.x() + qCos(rads-inner) * arrowTipLen,
                        mP1.y() + qSin(rads-inner) * arrowTipLen);
    linePoly << QPointF(mP1.x() + qCos(rads-outer) * arrowTipLen,
                        mP1.y() + qSin(rads-outer) * arrowTipLen);
    linePoly << mP1;

    if (!mVisible) painter->setOpacity(0);
    else painter->setOpacity(0.5);

    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPolygon(linePoly, Qt::WindingFill);
}

QRectF Line::boundingRect() const
{
    // Using the entire scene rect stops half-rendered lines staying on the screen
    return QRectF(-gBuffer, -gBuffer, gWidth+gBuffer, gHeight+gBuffer);
}
