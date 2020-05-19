#include "player.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <typeinfo>
#include "wall.h"
#include "point.h"
#include <QDebug>

Player::Player(short _x_maze, short _y_maze) {

    x_maze = _x_maze;
    y_maze = _y_maze;
    setRect(0, 0, width, height);
    setPos(x_maze + 225, y_maze + 375);

    move_dir.fill(false);

    connect(timer, &QTimer::timeout, this, &Player::move);
    timer->start(1000/30); //30 fps
}

void Player::keyPressEvent(QKeyEvent *event) {    

    //move_dir [UP, LEFT, DOWN, RIGHT]

    if (event->key() == Qt::Key_Up) move_dir.at(0) = true;
    if (event->key() == Qt::Key_Left) {
        move_dir.at(1) = true;
        tp = false;
    }
    if (event->key() == Qt::Key_Down) move_dir.at(2) = true;
    if (event->key() == Qt::Key_Right) {
        move_dir.at(3) = true;
        tp = false;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Up) move_dir.at(0) = false;
    if (event->key() == Qt::Key_Left) move_dir.at(1) = false;
    if (event->key() == Qt::Key_Down) move_dir.at(2) = false;
    if (event->key() == Qt::Key_Right) move_dir.at(3) = false;
}

//Pinta un pixel más!

//La idea es NO bloquear nada cuando se esté en los O.

//O ■ ■ ■ O
//■       ■
//■       ■
//■       ■
//O ■ ■ ■ O

void Player::stop(short x_wall, short y_wall, short width_wall, short height_wall) {

    short x_left = (x_wall - width), x_right = (x_wall + width_wall),
            y_up = (y_wall - height), y_down = (y_wall + height_wall);

    if (y() == y_up) {
        if ((x() != x_left) and (x() != x_right)) move_dir.at(2) = false;
        else if ((x() == x_left) and move_dir.at(2) and move_dir.at(3)) move_dir.at(2) = false;
        else if ((x() == x_right) and move_dir.at(2) and move_dir.at(1)) move_dir.at(2) = false;
    }
    else if (y() == y_down) {
        if ((x() != x_left) and (x() != x_right)) move_dir.at(0) = false;
        else if ((x() == x_left) and move_dir.at(0) and move_dir.at(3)) move_dir.at(0) = false;
        else if ((x() == x_right) and move_dir.at(0) and move_dir.at(1)) move_dir.at(0) = false;
    }
    else if (x() == x_right) move_dir.at(1) = false;
    else if (x() == x_left) move_dir.at(3) = false;

    //Esto causaba el bug de atravesar las paredes por las esquinas.

//    if ((y() == y_up) and (x() != x_left) and (x() != x_right)) move_dir.at(2) = false;
//    if ((y() == y_down) and (x() != x_left) and (x() != x_right)) move_dir.at(0) = false;

//    if ((x() == x_left) and (y() != y_up) and (y() != y_down)) move_dir.at(3) = false;
//    if ((x() == x_right) and (y() != y_up) and (y() != y_down)) move_dir.at(1) = false;
}

void Player::move() {

    QList <QGraphicsItem*> collisions = collidingItems();

    for (short i = 0; i < collisions.size(); i++) {

        //Genera una advertencia.
        //if (typeid(*(collisions[i])) == typeid(Wall)) stop(collisions[i]->x(), collisions[i]->y(), 25, 25);

        auto item = collisions[i];

        //LONGITUD DE LAS PAREDES.
        if (typeid(*item) == typeid(Wall)) stop(collisions[i]->x(), collisions[i]->y(), 25, 25);
        else if (typeid(*item) == typeid(Point)) {

            //Para poder bajar en la jerarquia de las clases, en éste caso
            //bajar de QGraphicsItem* a Point*.

            Point *point = dynamic_cast<Point*>(item);
            if (point->get_type() == 1) emit earn_point(100);
            else emit earn_point(300);

            scene()->removeItem(collisions[i]);
            delete collisions[i];
        }
    }

    short pixels = 5;
    if (move_dir.at(0)) setPos(x(), y() - pixels);
    if (move_dir.at(1)) setPos(x() - pixels, y());
    if (move_dir.at(2)) setPos(x(), y() + pixels);
    if (move_dir.at(3)) setPos(x() + pixels, y());

    if ((x() == (x_maze + 475)) and (y() == (y_maze + 225)) and !tp) {
        setPos(x_maze - 25, y());
        tp = true;
    }
    else if ((x() == (x_maze - 25)) and (y() == (y_maze + 225)) and !tp) {
        setPos(x_maze + 475, y());
        tp = true;
    }
}













































