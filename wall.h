#ifndef WALL_H
#define WALL_H

#include <QGraphicsPixmapItem>

class Wall: public QGraphicsPixmapItem {
private:
    QPixmap *image;

    QRectF boundingRect() const;

public:
    Wall(short x, short y, short type);
    ~Wall() {delete image;};

};

#endif // WALL_H
