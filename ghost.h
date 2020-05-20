#ifndef GHOST_H
#define GHOST_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <array>
#include <math.h>
#include <wall.h>

class Ghost: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    //-------------------------------------------------------------------GENERAL

    short width = 25, height = 25, x_maze, y_maze, state = 0;

    QRectF boundingRect() const;

    //----------------------------------------------------------------MOVIMIENTO

    float pixels = 5;
    std::array<bool, 4> move_dir;
    QTimer *move_timer = new QTimer;
    //La variable dir es la dirección inicial del fantasma.
    short x_tar, y_tar, gap[4] = {0, -1, 0, 1}, dir = 2;

    void choose_dir();
    float calculate_dist(short x, short y, short x_tar, short y_tar);
    void stop(short x_wall, short y_wall, short width_wall, short height_wall);

    //----------------------------------------------------------------COLISIONES

    QTimer  *scare_timer = new QTimer;
    QList <QGraphicsItem*> collisions;

    void fit_tile();

public:

    //-------------------------------------------------------------------GENERAL

    Ghost(short _x_maze, short _y_maze);
    ~Ghost() {delete move_timer; delete scare_timer;};

    //----------------------------------------------------------------COLISIONES

    void go_home();
    short get_state() {return state;};

public slots:

    //----------------------------------------------------------------MOVIMIENTO

    void move();
    void update_target(short _x_tar, short _y_tar);

    //----------------------------------------------------------------COLISIONES

    void scared_ghost();
    void normal_ghost();
};

#endif // GHOST_H
