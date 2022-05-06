#include <QGraphicsBlurEffect>
#include <QGraphicsItem>

class Blur : public QGraphicsBlurEffect
{
public:
    Blur(QGraphicsItem* item);

    void draw(QPainter *painter) override;

    void adjustForItem();
    void setRadius(qreal radius) { mRadius = radius; }

private:
    qreal mRadius;
};