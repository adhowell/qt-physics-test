#include <QGraphicsItem>

/**
 * Line for drawing the velocity vector when creating a new ball
 */
class Line : public QGraphicsItem {
public:
    Line() : mP1(QPointF(0 ,0)), mP2(QPointF(0, 0)) {};
    enum { Type = 5 };
    int type() const override { return Type; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QRectF boundingRect() const override;

    /**
     * Manual instruction for scene to re-paint the line
     */
    void update() { prepareGeometryChange(); }

    /**
     * Sets the start and end points of the line to the given position.
     * @param p1 position to use for both points
     */
    void updateBoth(QPointF p1) { mP1 = p1; mP2 = p1; }
    void updateEnd(QPointF p2) { mP2 = p2; }

    void setVisibility(bool isVisible) { mVisible = isVisible; }
    bool getVisibility() const { return mVisible; }

    QLineF getLine() const { return QLineF(mP2, mP1); }
    QPointF getStartPos() const { return mP1; }

private:
    bool mVisible = false;
    QPointF mP1;
    QPointF mP2;
};