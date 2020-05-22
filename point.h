#ifndef POINT_H
#define POINT_H

#include <QGraphicsPixmapItem>
#include <QTimer>

class Point: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    short type;
    short width = 3, height = 3;
    QPixmap *image1, *image2;
    bool bool_point = false;
    QTimer *timer;

    QRectF boundingRect() const;

public:
    Point(short x, short y, short _type = 1);
    ~Point();;
    short get_type() {return type;};

public slots:
    void animate();
};

#endif // POINT_H
