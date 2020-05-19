#include "point.h"

Point::Point(short x, short y) {
    setRect(0, 0, width, height);
    setPos(x - width/2, y - height/2);
}
