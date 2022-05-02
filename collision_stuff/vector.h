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
     * @param vec
     */
    Vector operator-(Vector vec) const;

    /**
     * Add the given vector to this vector.
     * @param vec
     */
    Vector operator+=(Vector vec);

    /**
     * Increases the magnitude (NOT the component
     * directions) of this vector by the given scalar.
     * @param scalar
     */
    void operator+=(qreal scalar);

    /**
     * Return the dot product of this vector and the
     * given vector.
     * @param vec
     */
    qreal operator*(Vector vec) const;

    /**
     * Return a vector with the given scalar product.
     * @param scalar
     */
    Vector operator*(qreal scalar) const;

    /**
     * Multiply this vector by the given scalar.
     * @param scalar
     */
    void operator*=(qreal scalar);

    /**
     * Reflects this vector about a vector with the given radial.
     * @param rad radial (radians) to flip about.
     */
    void reflectAbout(qreal rad);

    /**
     * Set all member values to zero;
     */
    void reset();

    /**
     * Sets the magnitude of the vector to the given scalar value.
     * @param scalar
     */
    void setSize(qreal scalar);

    qreal getSize() const { return mSize; }

private:
    qreal mX;
    qreal mY;
    qreal mAtan2;
    qreal mSize;
};