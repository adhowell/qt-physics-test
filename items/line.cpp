#include "line.h"
#include "../global_config.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>

void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    QPen pen;

    // Length-dependent colour
    QLineF l = QLineF(mP1, mP2);
    qreal normLen = qMin(l.length()/300.0, 1.0);
    pen.setColor(QColor(int(255*normLen), 0, int(255*(1-normLen))));

    if (!mVisible) painter->setOpacity(0);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawLine(l);
}

QRectF Line::boundingRect() const
{
    // Using the entire scene rect stops half-rendered lines staying on the screen
    return QRectF(-gBuffer, -gBuffer, gWidth+gBuffer, gHeight+gBuffer);
}
