#include "player.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>
#include <typeinfo>
#include "wall.h"

Player::Player(short x,short y) {

    setRect(0, 0, width, height);
    setPos(x - width/2, y - height/2);

    move_dir.fill(false);

    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
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
    //LONGITUD DE LAS PAREDES.
    for (auto it = collisions.cbegin(); it != collisions.cend(); it++) stop((*it)->x(), (*it)->y(), 25, 25);

    short pixels = 5;
    if (move_dir.at(0)) setPos(x(), y() - pixels);
    if (move_dir.at(1)) setPos(x() - pixels, y());
    if (move_dir.at(2)) setPos(x(), y() + pixels);
    if (move_dir.at(3)) setPos(x() + pixels, y());

    if ((x() == (162 + 475)) and (y() == (10 + 225)) and !tp) {
        setPos(162 - 25, y());
        tp = true;
    }
    else if ((x() == (162 - 25)) and (y() == (10 + 225)) and !tp) {
        setPos(162 + 475, y());
        tp = true;
    }
}













































