#ifndef POINT_H
#define POINT_H

#include <QGraphicsRectItem>

class Point: public QGraphicsRectItem {
private:
    short width = 5, height = 5;
public:
    Point(short x, short y);
    ~Point() {};
};

#endif // POINT_H
