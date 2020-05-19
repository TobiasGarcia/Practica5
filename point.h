#ifndef POINT_H
#define POINT_H

#include <QGraphicsRectItem>

class Point: public QGraphicsRectItem {
private:
    short type;
    short width = 3, height = 3;
public:
    Point(short x, short y, short _type = 1);
    ~Point() {};
    short get_type() {return type;};
};

#endif // POINT_H
