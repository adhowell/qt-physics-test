#include <QtWidgets>
#include <QtMath>

#pragma once

/**
 * Represents a normalised vector
 */
class NormVector {
public:
    NormVector(qreal dx, qreal dy);

    /**
     * Return the radial (radians) of the normalised vector.
     */
    qreal atan2() const { return mAtan2; }

private:
    qreal mAtan2;
};

/**
 * Representation for handling vector addition & reflection
 */
class Vector {
public:
    /**
     * Construct vector with a line representing direction only
     * (i.e. normalised vector) and a velocity.
     * @param l line representing direction of travel
     * @param vel velocity
     */
    Vector(QLineF l, qreal vel);

    /**
     * Construct vector with delta-x and delta-y.
     * @param dx delta x
     * @param dy delta y
     */
    Vector(qreal dx, qreal dy);

    QPointF getPosDelta(qreal deltaT) { return QPointF(mSize * deltaT * qCos(mAtan2),
                                                       mSize * deltaT * qSin(mAtan2)); }

    /**
     * Return a new vector created by subtracting the
     * given vector from this vector.
     * @param vec vector to subtract
     */
    Vector operator-(Vector vec) const;

    /**
     * Return the dot product of this vector and the
     * given vector.
     * @param vec vector to take dot product with
     */
    qreal operator*(Vector vec) const;

    /**
     * Return a vector with the given scalar product.
     * @param scalar
     */
    Vector operator*(qreal scalar) const;

    /**
     * Reflects this vector about a vector with the given radial.
     * @param rad radial (radians) to flip about.
     */
    void reflectAbout(qreal rad);

    qreal getSize() const { return mSize; }

private:
    qreal mX;
    qreal mY;
    qreal mAtan2;
    qreal mSize;
};