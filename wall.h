#ifndef WALL_H
#define WALL_H

#include <QGraphicsPixmapItem>
#include <QDebug>

class Wall: public QGraphicsPixmapItem {
private:
    short width = 25, height = 25;
    QPixmap *image;

    QRectF boundingRect() const;

public:
    Wall(short x, short y, short type);
    ~Wall() {delete image;};
};

#endif // WALL_H
