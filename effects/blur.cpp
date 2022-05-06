#include "blur.h"

Blur::Blur(QGraphicsItem *item) : QGraphicsBlurEffect()
{
}

void Blur::adjustForItem()
{
    setBlurRadius(mRadius);
}

void Blur::draw(QPainter *painter)
{
    adjustForItem();
    QGraphicsBlurEffect::draw(painter);
}