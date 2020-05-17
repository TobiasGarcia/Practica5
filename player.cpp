#include "player.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QTimer>
#include <QDebug>

Player::Player(short scene_w,short scene_h) {

    short width = 100, height = 100;
    setRect(scene_w/2 - width/2, scene_h/2 - height/2, 100, 100);

    move_dir.fill(false);

    QTimer *timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(50);
}

void Player::keyPressEvent(QKeyEvent *event) {

    //move_dir [UP, LEFT, DOWN, RIGHT]

    if (event->key() == Qt::Key_Up) move_dir.at(0) = true;
    if (event->key() == Qt::Key_Left) move_dir.at(1) = true;
    if (event->key() == Qt::Key_Down) move_dir.at(2) = true;
    if (event->key() == Qt::Key_Right) move_dir.at(3) = true;
}

void Player::keyReleaseEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Up) move_dir.at(0) = false;
    if (event->key() == Qt::Key_Left) move_dir.at(1) = false;
    if (event->key() == Qt::Key_Down) move_dir.at(2) = false;
    if (event->key() == Qt::Key_Right) move_dir.at(3) = false;
}

void Player::move() {

    if (move_dir.at(0)) setPos(x(), y() - 10);
    if (move_dir.at(1)) setPos(x() - 10, y());
    if (move_dir.at(2)) setPos(x(), y() + 10);
    if (move_dir.at(3)) setPos(x() + 10, y());
}
