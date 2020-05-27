#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <typeinfo>
#include <QTimer>
#include <array>
#include "wall.h"
#include "point.h"
#include "ghost.h"

class Player: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

//-----------------------------------------------------------------------------------------
//-----------------------------------------GENERAL-----------------------------------------
//-----------------------------------------------------------------------------------------

private:
    bool is_playing;
    short width, height, points_left, normal_ghosts;

    QRectF boundingRect() const;

public:

    Player();
    void initialize();
    void focusOutEvent(QFocusEvent *event);
    ~Player();

signals:
    void begin();
    void no_points_left();
    void earn_point(short points);

//-----------------------------------------------------------------------------------------
//---------------------------------MOVIMIENTO Y COLISIONES---------------------------------
//-----------------------------------------------------------------------------------------

private:
    bool tp, freeze;
    QTimer *move_timer;
    QList <QGraphicsItem*> collisions;
    std::array<bool, 4> pressed_dir, move_dir;
    short pixels, last_presesed, dir, gap[4] = {0, -1, 0, 1};

    void stop(short x_wall, short y_wall);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    //NOTA: En los arrays pressed_dir y move_dir, la correspondencia entre las
    //direcciones y los índices es la siguiente:

    //0: Arriba
    //1: Izquierda
    //2: Abajo
    //3: Derecha

public:
    void set_freeze(bool _freeze) {freeze = _freeze;};

signals:
    void scare_ghosts();
    void touched_ghost();
    void new_target(short x_pac, short y_pac, short dir_pac);

public slots:
    void move();

//-----------------------------------------------------------------------------------------
//----------------------------------------IMÁGENES-----------------------------------------
//-----------------------------------------------------------------------------------------

private:
    QPixmap *script;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    short num_script;

    void win_animation();

public slots:
    void lose_animation();

//-----------------------------------------------------------------------------------------
//----------------------------------------SONIDOS------------------------------------------
//-----------------------------------------------------------------------------------------

private:
    short eaten_id;
    QSoundEffect *scare_sound, *eat_ghost_sound, *only_eyes_sound, *ghosts_walk_sound, *waka_sound;

public slots:
    void normal_ghost(short id);

};

#endif // PLAYER_H
