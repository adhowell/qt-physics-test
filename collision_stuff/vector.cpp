#include "vector.h"

NormVector::NormVector(qreal dx, qreal dy)
{
    mAtan2 = qAtan2(dy, dx);
    qreal dist = qSqrt(qPow(dx, 2.0) + qPow(dy, 2.0));
    mNormX = dist > 0 ? dx / dist : 0;
    mNormY = dist > 0 ? dy / dist : 0;
}

NormVector::NormVector(QLineF l)
{
    mAtan2 = qAtan2(l.y2()-l.y1(), l.x2()-l.x1());
    mNormX = l.length() > 0 ? (l.x2()-l.x1()) / l.length() : 0;
    mNormY = l.length() > 0 ? (l.y2()-l.y1()) / l.length() : 0;
}

void NormVector::operator+=(qreal rad)
{
    qreal dx = qCos(rad) + mNormX;
    qreal dy = qSin(rad) + mNormY;
    mAtan2 = qAtan2(dy, dx);
    qreal dist = qSqrt(qPow(dx, 2.0) + qPow(dy, 2.0));
    mNormX = dist > 0 ? dx / dist : 0;
    mNormY = dist > 0 ? dy / dist : 0;
}

void NormVector::operator+=(NormVector vec)
{
    qreal dx = vec.mNormX + mNormX;
    qreal dy = vec.mNormY + mNormY;
    mAtan2 = qAtan2(dy, dx);
    qreal dist = qSqrt(qPow(dx, 2.0) + qPow(dy, 2.0));
    mNormX = dist > 0 ? dx / dist : 0;
    mNormY = dist > 0 ? dy / dist : 0;
}

void NormVector::reflectAbout(qreal rad)
{
    qreal otherX = qCos(rad);
    qreal otherY = qSin(rad);
    qreal dot = mNormX*otherX + mNormY*otherY;
    mNormX = mNormX - 2.0*dot*otherX;
    mNormY = mNormY - 2.0*dot*otherY;
    mAtan2 = qAtan2(mNormY, mNormX);
}