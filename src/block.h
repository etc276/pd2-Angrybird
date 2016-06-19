#ifndef BLOCK_H
#define BLOCK_H

#include <gameitem.h>
#include <QPixmap>
#include <QGraphicsScene>
#include <QTimer>

#define BLOCK_DENSITY 10.0f
#define BLOCK_FRICTION 0.2f
#define BLOCK_RESTITUTION 0.5f

class Block : public GameItem
{
public:
    Block(float x, float y, float w, float d, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene);
    int getScore();
    bool checkX();
    bool checkY();
    void change();
};

#endif // BLOCK_H
