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
     * @param dist radial (radians) to move along
     * @param rad
     */
    void posUpdate(qreal dist, qreal rad);

    /**
     * Update the radial direction the ball is moving along
     * by vector addition of a vector with the given radial.
     * @param rad radial (radians) to add to the direction vector.
     */
    void vectorUpdate(qreal rad);

    /**
     * Set the velocity of the ball to the given value.
     * @param newVel new velocity
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