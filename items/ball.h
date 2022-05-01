#include <QGraphicsItem>
#include "../collision_stuff/vector.h"
#include "wall.h"

/**
 * Physics-enabled ball graphics item
 */
class Ball : public QGraphicsItem {
public:
    Ball(QColor color, qreal x, qreal y, VelocityVector v);
    Ball(QColor color, QPointF p, VelocityVector v);
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
     * by vector addition of a vector with the given radial.
     * @param rad radial (radians) to add to the direction vector.
     */
    void vectorUpdate(qreal rad);

    /**
     * Update the radial direction the ball is moving along
     * by reflecting the current travel vector about a vector
     * with the given radial.
     * @param rad radial (radians) to flip about.
     */
    void vectorReflect(qreal rad);

    /**
     * Swap velocity with the given ball. Used in
     * elastic equal-mass collisions.
     * @param b The ball object to swap velocities with.
     */
    void swapVel(Ball* b);

    /**
     * Set the velocity of the ball to the given value.
     * @param newVel new velocity.
     */
    void velUpdate(qreal newVel) { mV.setVel(newVel); }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    qreal getVel() const { return mV.getVel(); }

    qreal distance(Ball* b) const;
    qreal distance(Wall* w) const;
    qreal atan2(Ball* b) const;

    static int sRadius;

private:
    QColor mColor;
    QPointF mP;
    VelocityVector mV;
};