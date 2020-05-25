#include "point.h"

QRectF Point::boundingRect() const {
    if (type == 1) return QRectF(0, 0, 3, 3);
    else return QRectF(0, 0, 11, 11);
}

Point::Point(short x, short y, short _type) {

    type = _type;

    bool_point = true;

    if (type == 1) {
        image1 = new QPixmap(":/images/resources/images/points/point.png");
        setPos(x, y);
        setPixmap(*image1);
    }
    else {
        image1 = new QPixmap(":/images/resources/images/points/big_point1.png");
        image2 = new QPixmap(":/images/resources/images/points/big_point2.png");
        setPixmap(*image1);
        setPos(x - 4, y - 4);

        timer = new QTimer;

        connect(timer, &QTimer::timeout, this, &Point::animate);
        timer->start(200);
    }
}

Point::~Point() {
    delete image1;
    if (type == 2) {
        delete timer;
        delete image2;
    }
}

void Point::animate() {
    if (bool_point) {
        bool_point = false;
        setPixmap(*image2);
        setPos(x() - 2, y() - 2);
    }
    else {
        bool_point = true;
        setPixmap(*image1);
        setPos(x() + 2, y() + 2);
    }
}
