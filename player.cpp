#include "player.h"
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QDebug>
#include <typeinfo>
#include "wall.h"
#include "point.h"
#include <QDebug>

QRectF Player::boundingRect() const {
    return QRectF(-1, -1, 27, 27);
}

Player::Player(short _x_maze, short _y_maze) {

    x_maze = _x_maze;
    y_maze = _y_maze;

    script = new QPixmap[15];
    script[0] = QPixmap(":/images/resources/images/pacman/pacman1.png");
    script[1] = QPixmap(":/images/resources/images/pacman/pacman2.png");
    script[2] = QPixmap(":/images/resources/images/pacman/pacman3.png");
    script[3] = QPixmap(":/images/resources/images/pacman/pacman4.png");
    script[4] = QPixmap(":/images/resources/images/pacman/pacman5.png");
    script[5] = QPixmap(":/images/resources/images/pacman/pacman6.png");
    script[6] = QPixmap(":/images/resources/images/pacman/pacman7.png");
    script[7] = QPixmap(":/images/resources/images/pacman/pacman8.png");
    script[8] = QPixmap(":/images/resources/images/pacman/pacman9.png");
    script[9] = QPixmap(":/images/resources/images/pacman/pacman10.png");
    script[10] = QPixmap(":/images/resources/images/pacman/pacman11.png");
    script[11] = QPixmap(":/images/resources/images/pacman/pacman12.png");
    script[12] = QPixmap(":/images/resources/images/pacman/pacman13.png");
    script[13] = QPixmap(":/images/resources/images/pacman/pacman14.png");
    script[14] = QPixmap(":/images/resources/images/walls/empty.png");

    initialize();

    connect(timer, &QTimer::timeout, this, &Player::move);
    timer->start(1000/30); //30 fps
}

void Player::focusOutEvent(QFocusEvent *event) {
    Q_UNUSED(event);
    setFocus();
}

void Player::initialize() {

    freeze = true;

    num_script = 0;
    dir = 0;
    last_presesed = 0;
    tp = false;

    pressed_dir.fill(false);
    move_dir.fill(false);

    setPixmap(script[0]);
    setPos(x_maze + 225, y_maze + 375);
}

void Player::keyPressEvent(QKeyEvent *event) {

    if (!is_playing) {
        is_playing = true;
        emit begin();
    }

    //move_dir [UP, LEFT, DOWN, RIGHT]

    if ((event->key() == Qt::Key_Up) and !event->isAutoRepeat()) {
        pressed_dir.at(0) = true;
        last_presesed = 0;
    }
    if ((event->key() == Qt::Key_Left) and !event->isAutoRepeat()) {
        pressed_dir.at(1) = true;
        last_presesed = 1;
        tp = false;
    }
    if ((event->key() == Qt::Key_Down) and !event->isAutoRepeat()) {
        pressed_dir.at(2) = true;
        last_presesed = 2;
    }
    if ((event->key() == Qt::Key_Right) and !event->isAutoRepeat()) {
        pressed_dir.at(3) = true;
        last_presesed = 3;
        tp = false;
    }
}

void Player::keyReleaseEvent(QKeyEvent *event) {

    if ((event->key() == Qt::Key_Up) and !event->isAutoRepeat()) pressed_dir.at(0) = false;
    if ((event->key() == Qt::Key_Left) and !event->isAutoRepeat()) pressed_dir.at(1) = false;
    if ((event->key() == Qt::Key_Down) and !event->isAutoRepeat()) pressed_dir.at(2) = false;
    if ((event->key() == Qt::Key_Right) and !event->isAutoRepeat()) pressed_dir.at(3) = false;
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

//    if (y() == y_up) {
//        if ((x() != x_left) and (x() != x_right)) move_dir.at(2) = false;
//        else if ((x() == x_left) and move_dir.at(2) and move_dir.at(3)) move_dir.at(2) = false;
//        else if ((x() == x_right) and move_dir.at(2) and move_dir.at(1)) move_dir.at(2) = false;
//    }
//    else if (y() == y_down) {
//        if ((x() != x_left) and (x() != x_right)) move_dir.at(0) = false;
//        else if ((x() == x_left) and move_dir.at(0) and move_dir.at(3)) move_dir.at(0) = false;
//        else if ((x() == x_right) and move_dir.at(0) and move_dir.at(1)) move_dir.at(0) = false;
//    }
//    else if (x() == x_right) move_dir.at(1) = false;
//    else if (x() == x_left) move_dir.at(3) = false;

    //Esto causaba el bug de atravesar las paredes por las esquinas.

    //qDebug() << x_wall << "   " << y_wall << "        " <<  x() << y();

    if ((y() == y_up) and (x() != x_left) and (x() != x_right)) move_dir.at(2) = false;
    if ((y() == y_down) and (x() != x_left) and (x() != x_right)) move_dir.at(0) = false;

    if ((x() == x_left) and (y() != y_up) and (y() != y_down)) move_dir.at(3) = false;
    if ((x() == x_right) and (y() != y_up) and (y() != y_down)) move_dir.at(1) = false;
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->translate(13, 13);
    painter->rotate(-dir*90);
    painter->translate(-13, -13);
    painter->drawPixmap(2, 2, script[num_script]);

//    if (state == 0) {
//        painter->drawPixmap(0, 0, sheets[sheet_bool]);
//        painter->drawPixmap(0, 0, eyes[dir]);
//    }
//    else if (state == 1) painter->drawPixmap(0, 0, scared_ghost[sheet_bool]);
//    else painter->drawPixmap(0, 0, eyes[dir]);
}

void Player::move() {

    if (freeze) return;

    move_dir = pressed_dir;

    collisions = collidingItems(Qt::IntersectsItemBoundingRect);
    for (short i = 0; i < collisions.size(); i++) {

        //Genera una advertencia.
        //if (typeid(*(collisions[i])) == typeid(Wall)) stop(collisions[i]->x(), collisions[i]->y(), 25, 25);

        auto item = collisions.at(i);
        //LONGITUD DE LAS PAREDES.
        if (typeid(*item) == typeid(Wall)) stop(collisions.at(i)->x(), collisions.at(i)->y(), 25, 25);
        else if (typeid(*item) == typeid(Point)) {

            //Para poder bajar en la jerarquia de las clases, en éste caso
            //bajar de QGraphicsItem* a Point*.

            Point *point = dynamic_cast<Point*>(item);
            if (point->get_type() == 1) emit earn_point(10);
            else {
                emit earn_point(50);
                emit scare_ghosts();
            }

            scene()->removeItem(collisions.at(i));
            delete collisions.at(i);

            points_left--;
            if (points_left == 0) {
                emit no_points_left();
                return;
            }
        }
        else if ((typeid(*item) == typeid(Ghost)) and (calculate_dist(collisions.at(i)->x(), collisions.at(i)->y(), x(), y()) < 15)) {
            Ghost *ghost = dynamic_cast<Ghost*>(item);
            if (ghost->get_state() == 1) {
                emit earn_point(200);
                ghost->go_home();
            }
            else if (ghost->get_state() == 0) {
                emit touched_ghost();
                return;
            }
        }
    }

    //num_script = (num_script + 1)%3 debe ir dentro de los condicionales
    //para que solo suceda cuando alguno salio true, si lo dejamos por fuera
    //siempre sucedería.

    if (move_dir.at(last_presesed)) {
        setPos(x() + pixels*gap[last_presesed], y() + pixels*gap[(last_presesed + 1)%4]);
        num_script = (num_script + 1)%3;
        dir = last_presesed;
    }
    else if (move_dir.at(0)) {
        setPos(x(), y() - pixels);
        num_script = (num_script + 1)%3;
        dir = 0;
    }
    else if (move_dir.at(1)) {
        setPos(x() - pixels, y());
        num_script = (num_script + 1)%3;
        dir = 1;
    }
    else if (move_dir.at(2)) {
        setPos(x(), y() + pixels);
        num_script = (num_script + 1)%3;
        dir = 2;
    }
    else if (move_dir.at(3)) {
        setPos(x() + pixels, y());
        dir = 3;
    }

    if ((x() == (x_maze + 475)) and (y() == (y_maze + 225)) and !tp) {
        setPos(x_maze - 25, y());
        tp = true;
    }
    else if ((x() == (x_maze - 25)) and (y() == (y_maze + 225)) and !tp) {
        setPos(x_maze + 475, y());
        tp = true;
    }

    //Enviamos la dirección hacia donde quiere ir el jugador.

    emit new_target(x(), y(), last_presesed);
}

void delay(short mili) {
    QTime sleep_time = QTime::currentTime().addMSecs(mili);
    while (QTime::currentTime() < sleep_time) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void Player::lose_animation() {

    for (num_script = 0; num_script < 14; num_script++) {
        update();
        delay(150);
    }
    update();
    is_playing = false;
}













































