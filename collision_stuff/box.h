#include <QtWidgets>
#include "../items/wall.h"

/**
 * Used to create a box of four wall objects
 */
class Box {
public:
    Box(qreal x, qreal y);
    QVector<Wall*> getWalls() { return mWalls; }

    static int sWidth;
    static int sHeight;

private:
    QVector<Wall*> mWalls;
};