#ifndef WALL_H
#define WALL_H

#include <QGraphicsRectItem>
#include <array>

class Wall: public QGraphicsRectItem {
private:
    short width = 30, height = 30;
public:
    Wall(short x, short y);
};

#endif // WALL_H
