#ifndef WALL_H
#define WALL_H

#include <QGraphicsPixmapItem>

//Ésta clase modela las paredes del laberinto.

class Wall: public QGraphicsPixmapItem {
private:
    QPixmap *image;

    QRectF boundingRect() const;

public:
    Wall(short x, short y, short type);
    ~Wall() {delete image;};

};

#endif // WALL_H
