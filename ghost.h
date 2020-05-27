#ifndef GHOST_H
#define GHOST_H

#include <QGraphicsPixmapItem>
#include <QSoundEffect>
#include <QPainter>
#include <QTimer>
#include <array>
#include "wall.h"
#include "general.h"

class Ghost: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

//-----------------------------------------------------------------------------------------
//-----------------------------------------GENERAL-----------------------------------------
//-----------------------------------------------------------------------------------------

private:
    short width, height, state, id;

    QRectF boundingRect() const;

public:
    void initialize();
    short get_state() {return state;};
    short get_id() {return id;};
    Ghost(QPixmap *_eyes, QPixmap *_scared_ghost, short _id);
    ~Ghost();

//-----------------------------------------------------------------------------------------
//---------------------------------MOVIMIENTO Y COLISIONES---------------------------------
//-----------------------------------------------------------------------------------------

private:
    bool freeze;
    float pixels;
    std::array<bool, 4> move_dir;
    QTimer *move_timer, *scare_timer;
    QList <QGraphicsItem*> collisions;
    short x_tar, y_tar, dir, gap[4] = {0, -1, 0, 1};

    void fit_tile();
    void choose_dir();
    void stop(short x_wall, short y_wall);
    void blinky_target(short x_pac, short y_pac);
    void pinky_target(short x_pac, short y_pac, short dir_pac);
    void clyde_target(short x_pac, short y_pac);

    //NOTA: El método inky_target() es un slot, esto es gracias a que
    //el cálculo del objetivo de Inky depende de la posición de Blinky.

public:
    void go_home();
#if GHOSTS_TARGETS == 1
    QGraphicsPixmapItem *target;
#endif

public slots:
    void move();
    void scare();
    void normal_ghost();
    void set_freeze(bool _freeze);
    void update_target(short x_pac, short y_pac, short dir_pac);
    void inky_target(short x_pac, short y_pac, short x_blin, short y_blin, short dir_pac);

signals:
    void blinky_pos(short x_pac, short y_pac, short x_blin, short y_blin, short dir_pac);

//-----------------------------------------------------------------------------------------
//----------------------------------------IMÁGENES-----------------------------------------
//-----------------------------------------------------------------------------------------

private:
    bool sheet_bool;
    QTimer *animation_timer;
    QPixmap *eyes, *scared_ghost, *sheets;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public slots:
    void animate_ghost() {sheet_bool = !sheet_bool;update();};

//-----------------------------------------------------------------------------------------
//----------------------------------------SONIDOS------------------------------------------
//-----------------------------------------------------------------------------------------

signals:
    void back_normal(short id);

};

#endif // GHOST_H
