#include "box.h"

int Box::sWidth = 25;
int Box::sHeight = 25;

Box::Box(qreal x, qreal y)
{
    mWalls << new Wall(QLineF(QPointF(x+sWidth*0.5, y-sHeight*0.5),
                                 QPointF(x-sWidth*0.5, y-sHeight*0.5)));
    mWalls << new Wall(QLineF(QPointF(x-sWidth*0.5, y-sHeight*0.5),
                                 QPointF(x-sWidth*0.5, y+sHeight*0.5)));
    mWalls << new Wall(QLineF(QPointF(x-sWidth*0.5, y+sHeight*0.5),
                                 QPointF(x+sWidth*0.5, y+sHeight*0.5)));
    mWalls << new Wall(QLineF(QPointF(x+sWidth*0.5, y+sHeight*0.5),
                                 QPointF(x+sWidth*0.5, y-sHeight*0.5)));
}