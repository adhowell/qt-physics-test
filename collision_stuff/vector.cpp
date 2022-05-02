#include "vector.h"

NormVector::NormVector(qreal dx, qreal dy)
{
    mAtan2 = qAtan2(dy, dx);
}

Vector::Vector(QLineF l, qreal vel)
{
    mSize = vel;
    mAtan2 = qAtan2(l.y2()-l.y1(), l.x2()-l.x1());
    mX = qCos(mAtan2) * vel;
    mY = qSin(mAtan2) * vel;
}

Vector::Vector(qreal dx, qreal dy)
{
    mSize = qSqrt(qPow(dx, 2.0) + qPow(dy, 2.0));
    mAtan2 = qAtan2(dy, dx);
    mX = dx;
    mY = dy;
}

void Vector::reflectAbout(qreal rad)
{
    qreal otherX = qCos(rad);
    qreal otherY = qSin(rad);
    qreal dot = mX*otherX + mY*otherY;
    mX -= 2.0*dot*otherX;
    mY -= 2.0*dot*otherY;
    mAtan2 = qAtan2(mY, mX);
}

Vector Vector::operator-(Vector vec) const
{
    return Vector(mX - vec.mX, mY - vec.mY);
}

qreal Vector::operator*(Vector vec) const
{
    return mX*vec.mX + mY*vec.mY;
}

Vector Vector::operator*(qreal scalar) const
{
    return Vector(mX*scalar, mY*scalar);
}