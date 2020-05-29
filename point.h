#ifndef POINT_H
#define POINT_H

#include <QGraphicsPixmapItem>
#include <QTimer>

//Ésta clase modela los puntos colocados en el laberinto.

class Point: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:
    short type;
    QTimer *timer;
    bool bool_point;
    QPixmap *image1, *image2;

    QRectF boundingRect() const;

public:
    short get_type() {return type;};
    Point(short x, short y, short _type = 1);
    ~Point();

public slots:
    void animate();
};

#endif // POINT_H
