#include "ball.h"

qreal Ball::sMinRadius = 5.0;
qreal Ball::sMaxRadius = 15.0;
qreal Ball::sMinMass = 1.0;
qreal Ball::sMaxMass = 10.0;

Ball::Ball(QColor color, qreal x, qreal y, Vector v, qreal mass, qreal radius)
: mColor(color), mP(QPointF(x, y)), mV(v), mM(mass), mR(radius), mA(Vector(0, 0))
{
    setPos(x-mR*0.5, y-mR*0.5);
}

Ball::Ball(QColor color, QPointF p, Vector v, qreal mass, qreal radius)
: mColor(color), mP(p), mV(v), mM(mass), mR(radius), mA(Vector(0, 0))
{
    setPos(p.x()-mR*0.5, p.y()-mR*0.5);
}

void Ball::posUpdate(qreal dist, qreal rad)
{
    mP += QPointF(dist * qCos(rad), dist * qSin(rad));
}

void Ball::vectorReflect(qreal rad)
{
    mV.reflectAbout(rad);
}

void Ball::advance(qreal deltaT)
{
    // Update the velocity vector with the acceleration vector
    mV += mA * deltaT;

    // Compute new position based on delta-t
    if (mV.getSize() > 50.0) mV.setSize(50.0);
    if (mV.getSize() > 0) {
        //mV.velocityDragAdjust();
        mP += mV.getPosDelta(deltaT);
    }
}

void Ball::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(widget);

    QPen pen;
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);

    pen.setColor(mColor);
    QColor brushColor;
    brushColor.setRgb(mColor.rgb());
    if (mM >= sMinMass)
        brushColor.setAlphaF((mM - sMinMass) / (sMaxMass - sMinMass));
    brush.setColor(brushColor);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(QRectF(-mR, -mR, mR*2.0, mR*2.0));
}

QRectF Ball::boundingRect() const
{
    // Making the bounding rect too big helps with rendering fast moving balls
    return QRectF(-mR*2, -mR*2, mR*4.0, mR*4.0);
}

qreal Ball::distance(Ball* b) const
{
    return qSqrt(qPow(mP.x()-b->mP.x(), 2.0) + qPow(mP.y()-b->mP.y(), 2.0));
}

qreal Ball::distance(Wall* w) const
{
    QLineF l = w->getLine();

    // Check if line endpoint inside circle
    qreal startDist = qSqrt(qPow(mP.x()-l.x1(), 2.0) + qPow(mP.y()-l.y1(), 2.0));
    if (startDist <= mR) return startDist;
    qreal endDist = qSqrt(qPow(mP.x()-l.x2(), 2.0) + qPow(mP.y()-l.y2(), 2.0));
    if (endDist <= mR) return endDist;

    // Get dot product of the line and circle
    qreal dot = ( ((mP.x()-l.x1())*(l.x2()-l.x1())) + ((mP.y()-l.y1())*(l.y2()-l.y1())) ) / qPow(l.length(),2);

    // Find the closest point on the line
    qreal closestX = l.x1() + (dot * (l.x2()-l.x1()));
    qreal closestY = l.y1() + (dot * (l.y2()-l.y1()));

    // Get distance from the closest point to the two ends of the line
    qreal d1 = qSqrt(qPow(closestX-l.x1(), 2.0) + qPow(closestY-l.y1(), 2.0));
    qreal d2 = qSqrt(qPow(closestX-l.x2(), 2.0) + qPow(closestY-l.y2(), 2.0));

    // Return the distance to the nearest end-point if the closest point isn't on the line
    if (d1+d2 < l.length() || d1+d2 > l.length())
        return qMin(startDist, endDist);

    // Get distance to closest point
    qreal distX = closestX - mP.x();
    qreal distY = closestY - mP.y();
    return qSqrt((distX*distX) + (distY*distY));
}

qreal Ball::atan2(Ball* b) const
{
    return qAtan2(mP.y()-b->mP.y(), mP.x()-b->mP.x());
};

void Ball::collide(Ball* b)
{
    Vector x1 = Vector(mP.x(), mP.y());
    Vector x2 = Vector(b->mP.x(), b->mP.y());
    Vector x12 = x1 - x2;
    Vector x21 = x2 - x1;
    Vector thisNewVec = mV - x12 * ( ((mV - b->mV) * (x12)) / qPow(x12.getSize(), 2.0) )
            * (2.0 * b->mM / (mM + b->mM));
    Vector otherNewVec = b->mV - x21 * ( ((b->mV - mV) * (x21)) / qPow(x21.getSize(), 2.0) )
            * (2.0 * mM / (mM + b->mM));
    mV = thisNewVec;
    b->mV = otherNewVec;
}

void Ball::addVelocityVector(Vector v)
{
    mV += v;
}

void Ball::velocityMultiply(qreal scalar)
{
    mV *= scalar;
}

void Ball::velocityAddition(qreal scalar)
{
    mV += scalar;
}

void Ball::addAccelerationVector(Vector a)
{
    mA += a;
}

void Ball::setAccelerationVector(Vector a)
{
    mA = a;
}