#ifndef WALL_H
#define WALL_H

#include <QGraphicsRectItem>

class Wall: public QGraphicsRectItem {
private:
    short width = 25, height = 25;
public:
    Wall(short x, short y);
    ~Wall();
};

#endif // WALL_H
