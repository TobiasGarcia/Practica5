#ifndef GHOST_H
#define GHOST_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <array>
#include <math.h>
#include "wall.h"
#include <QPainter>

class Ghost: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    //-------------------------------------------------------------------GENERAL

    short width = 25, height = 25, x_maze, y_maze, state = 0, id;

    QRectF boundingRect() const;

    //----------------------------------------------------------------MOVIMIENTO

    float pixels = 5;
    QTimer *move_timer;
    std::array<bool, 4> move_dir;
    short x_tar, y_tar, gap[4] = {0, -1, 0, 1}, dir = 0;

    void choose_dir();
    float calculate_dist(short x, short y, short x_tar, short y_tar);
    void stop(short x_wall, short y_wall, short width_wall, short height_wall);
    void blinky_target(short x_pac, short y_pac);
    void pinky_target(short x_pac, short y_pac, short dir_pac);
    void clyde_target(short x_pac, short y_pac);

    //----------------------------------------------------------------COLISIONES

    QTimer  *scare_timer;
    QList <QGraphicsItem*> collisions;

    void fit_tile();

    //------------------------------------------------------------------IMÁGENES

    bool sheet_bool = true;
    QTimer *animation_timer;
    QPixmap *eyes, *scared_ghost, *sheets;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:

    //QGraphicsPixmapItem *target;//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    //-------------------------------------------------------------------GENERAL

    Ghost(short _x_maze, short _y_maze, QPixmap *_eyes, QPixmap *_scared_ghost, short _id);
    ~Ghost();

    //----------------------------------------------------------------COLISIONES

    void go_home();
    short get_state() {return state;};

signals:

    void blinky_pos(short x_pac, short y_pac, short x_blin, short y_blin, short dir_pac);

public slots:

    //----------------------------------------------------------------MOVIMIENTO

    void move();
    void update_target(short x_pac, short y_pac, short dir_pac);
    void inky_target(short x_pac, short y_pac, short x_blin, short y_blin, short dir_pac);

    //----------------------------------------------------------------COLISIONES

    void scare();
    void normal_ghost();

    //------------------------------------------------------------------IMÁGENES

    void animate_ghost() {sheet_bool = !sheet_bool;};
};

#endif // GHOST_H
