#include <QtWidgets>
#include <QtMath>

#pragma once

/**
 * Represents a normalised vector
 */
class NormVector {
public:
    NormVector(qreal dx, qreal dy);
    explicit NormVector(QLineF l);

    /**
     * Equivalent to adding another normalised vector
     * of angle rad.
     * @param rad radial (radians) of other normalised vector
     */
    void operator+=(qreal rad);

    /**
     * Vector addition.
     * @param vec NormVector object to add
     */
    void operator+=(NormVector vec);

    /**
     * Return the radial (radians) of the normalised vector.
     */
    qreal atan2() const { return mAtan2; }

private:
    qreal mNormX;
    qreal mNormY;
    qreal mAtan2;
};

/**
 * Contains a NormVector for direction and an associated velocity.
 */
class VelocityVector {
public:
    VelocityVector(QLineF l, qreal vel) : mVec(l), mVel(vel) {};
    QPointF getPosDelta(qreal deltaT) { return QPointF(mVel * deltaT * qCos(mVec.atan2()),
                                                       mVel * deltaT * qSin(mVec.atan2())); }
    void operator+=(qreal rad) { mVec+=rad; }
    void operator+=(NormVector vec) { mVec+=vec; }

    qreal getVel() const { return mVel; }
    void setVel(qreal vel) { mVel = vel > 0.1 ? vel : 0; }

private:
    NormVector mVec;
    qreal mVel;
};