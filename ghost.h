#ifndef GHOST_H
#define GHOST_H

#include <QGraphicsRectItem>
#include <QTimer>
#include <array>
#include <math.h>
#include <wall.h>

class Ghost: public QObject, public QGraphicsRectItem {

    Q_OBJECT

private:
    std::array<bool, 4> move_dir;
    //---------------------------------DIR ES LA DIRECCIÓN INICIAL DEL FANTASMA
    short width = 25, height = 25, pixels = 5, x_tar, y_tar, dir = 2,
    gap[4] = {0, -1, 0, 1}, x_maze, y_maze;
    QTimer *timer = new QTimer;
    QList <QGraphicsItem*> collisions;

    float calculate_dist(short x, short y, short x_tar, short y_tar);
    void stop(short x_wall, short y_wall, short width_wall, short height_wall);
    void choose_dir();

public:
    Ghost(short _x_maze, short _y_maze);
    ~Ghost() {delete timer;};

public slots:
    void move();
    void update_target(short _x_tar, short _y_tar);
};

#endif // GHOST_H
