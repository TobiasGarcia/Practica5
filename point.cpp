#include "point.h"

Point::Point(short x, short y, short _type) {
    type = _type;
    if (type == 1) {
        setRect(0, 0, width, height);
        setPos(x, y);
    }
    else {
        setRect(0, 0, width + 4, height + 4);
        setPos(x - 2, y - 2);
    }
}
