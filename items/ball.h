#include <QGraphicsItem>
#include "../collision_stuff/vector.h"
#include "wall.h"

/**
 * Physics-enabled ball graphics item
 */
class Ball : public QGraphicsItem {
public:
    Ball(QColor color, qreal x, qreal y, Vector v);
    Ball(QColor color, QPointF p, Vector v);
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

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;
    void update() { setPos(mP); }

    qreal getVel() const { return mV.getSize(); }

    static int sRadius;

private:
    QColor mColor;
    QPointF mP;
    Vector mV;
};