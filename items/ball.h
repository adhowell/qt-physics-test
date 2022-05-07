#include <QGraphicsItem>
#include "../collision_stuff/vector.h"
#include "wall.h"

/**
 * Physics-enabled ball graphics item
 */
class Ball : public QGraphicsItem {
public:
    Ball(QColor color, qreal x, qreal y, Vector v, qreal mass, qreal radius);
    Ball(QColor color, QPointF p, Vector v, qreal mass, qreal radius);
    enum { Type = 3 };
    int type() const override { return Type; }

    /**
     * Update the position of the ball given the contained
     * velocity and direction vector, and the given delta-t.
     * @param deltaT time-step used to calculate delta-pos
     */
    void advance(qreal deltaT);

    /**
     * Move the ball by the given radial and distance.
     * @param dist distance to move the ball by
     * @param rad radial (radians) to move along
     */
    void posUpdate(qreal dist, qreal rad);

    /**
     * Update the radial direction the ball is moving along
     * by reflecting the current travel vector about a vector
     * with the given radial.
     * @param rad radial (radians) to flip about.
     */
    void vectorReflect(qreal rad);

    /**
     * Updates the velocity vector of this object and
     * of the given other ball, based on an elastic
     * collision.
     * @param b Ball object this ball is colliding with
     */
    void collide(Ball* b);

    /**
     * Return the radial angle of the vector between the
     * centre of this ball to the centre of the given
     * ball (in that direction).
     * @param b Other ball to calculate angle to
     */
    qreal atan2(Ball* b) const;

    /**
     * Return the distance from the centre of this ball
     * to the centre of the given ball.
     * @param b Other ball to calculate distance to
     */
    qreal distance(Ball* b) const;

    /**
     * Return the distance from the centre of this ball
     * to the closest point on the given wall.
     * @param w Wall to calculate distance to
     */
    qreal distance(Wall* w) const;

    /**
     * Adds the given velocity vector to the velocity
     * vector of this ball.
     * @param v vector to add
     */
    void addVelocityVector(Vector v);

    /**
     * Adds the given acceleration vector to the
     * acceleration vector of this ball.
     * @param v vector to add
     */
    void addAccelerationVector(Vector a);

    /**
     * Sets the given acceleration vector as the
     * acceleration vector of this ball.
     * @param v vector to add
     */
    void setAccelerationVector(Vector a);

    /**
     * Multiplies the velocity of this ball by
     * the given scalar.
     * @param scalar
     */
    void velocityMultiply(qreal scalar);

    /**
     * Adds the given scalar to the velocity
     * of this ball.
     * @param scalar
     */
    void velocityAddition(qreal scalar);

    void storePosition() { mOldP = mP; }

    void setTimeRemaining(qreal t) { mTimeRemaining = t; }

    void decrementTime(qreal deltaT) { mTimeRemaining -= deltaT; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void update() { setPos(mP); }

    qreal getRadius() const { return mR; }
    qreal getDeltaPos() const { return qSqrt(qPow(mP.x()-mOldP.x(), 2.0) + qPow(mP.y()-mOldP.y(), 2.0)); }
    qreal getEnergy() const { return 0.5*mM*qPow(mV.getSize(), 2.0); }
    qreal getTimeRemaining() { return mTimeRemaining; }
    qreal getVelocity() { return mV.getSize(); }

    static qreal sMinMass;
    static qreal sMaxMass;
    static qreal sMinRadius;
    static qreal sMaxRadius;

private:
    QColor mColor;
    QPointF mP; // Centre point
    Vector mV;  // Velocity vector
    Vector mA;  // Acceleration vector
    qreal mM;   // Mass
    qreal mR;   // Radius
    QPointF mOldP;  // Position at start of sub-epoch
    qreal mTimeRemaining; // For current sub-epoch
};