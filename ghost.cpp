#include "ghost.h"

//-------------------------------------------------------------------GENERAL

QRectF Ghost::boundingRect() const {
    return QRectF(0, 0, 25, 25);
}

Ghost::Ghost(short _x_maze, short _y_maze) {

    x_maze = _x_maze;
    y_maze = _y_maze;

    x_tar = x_maze + 225;
    y_tar = y_maze + 375;

    setPixmap(QPixmap(":/images/resources/images/blinky.png"));
    setPos(x_maze + 225, y_maze + 175);

    connect(move_timer, &QTimer::timeout, this, &Ghost::move);
    connect(scare_timer, &QTimer::timeout, this, &Ghost::normal_ghost);
    move_timer->start(1000/30);
}

//----------------------------------------------------------------MOVIMIENTO

float Ghost::calculate_dist(short x, short y, short x_tar, short y_tar) {

    //Calculamos la distancia para ir por líneas rectas.

    return (sqrt(pow(y_tar - y, 2) + pow(x_tar - x, 2)));
}

void Ghost::stop(short x_wall, short y_wall, short width_wall, short height_wall) {

    short x_left = (x_wall - width), x_right = (x_wall + width_wall),
            y_up = (y_wall - height), y_down = (y_wall + height_wall);

    if ((y() == y_up) and (x() != x_left) and (x() != x_right)) move_dir.at(2) = false;
    if ((y() == y_down) and (x() != x_left) and (x() != x_right)) move_dir.at(0) = false;
    if ((x() == x_left) and (y() != y_up) and (y() != y_down)) move_dir.at(3) = false;
    if ((x() == x_right) and (y() != y_up) and (y() != y_down)) move_dir.at(1) = false;
}

void Ghost::choose_dir() {

    short min;
    float min_dist = 2048, dist;

    for (short i = 0; i < 4; i++) {

        //No consideramos la dirección opuesta a la que se está moviendo.

        if ((i != (dir + 2)%4) and move_dir.at(i)) {

            dist = calculate_dist(x() + 25*gap[i], y() + 25*gap[(i + 1)%4], x_tar, y_tar);

            if (dist < min_dist) {
                min_dist = dist;
                min = i;
            }
        }
        move_dir.at(i) = false;
    }
    move_dir.at(min) = true;
    dir = min;
}

void Ghost::update_target(short _x_tar, short _y_tar) {

    //Lo colocamos en la posición de Pacman.

    if (!state) {
        x_tar = _x_tar;
        y_tar = _y_tar;
    }
}

void Ghost::move() {

    if ((state == 2) and ((x() == (x_maze + 225)) and (y() == (y_maze + 175)))) normal_ghost();

    if ((x() == (x_maze + 475)) and (y() == (y_maze + 225))) setPos(x_maze - 25, y());
    else if ((x() == (x_maze - 25)) and (y() == (y_maze + 225))) setPos(x_maze + 475, y());

    move_dir.fill(true);

    collisions = collidingItems(Qt::IntersectsItemBoundingRect);
    for (short i = 0; i < collisions.size(); i++) {
        auto item = collisions.at(i);
        //LONGITUD DE LAS PAREDES.
        if (typeid(*item) == typeid(Wall)) stop(collisions.at(i)->x(), collisions.at(i)->y(), 25, 25);
    }

    choose_dir();
    if (move_dir.at(0)) setPos(x(), y() - pixels);
    else if (move_dir.at(1)) setPos(x() - pixels, y());
    else if (move_dir.at(2)) setPos(x(), y() + pixels);
    else setPos(x() + pixels, y());
}

//----------------------------------------------------------------COLISIONES

void Ghost::fit_tile() {

    short x_rem = (short(x()) - x_maze)%25, y_rem = (short(y()) - y_maze)%25;

    if (x_rem < 13) setPos(short(x()) - x_rem, y());
    else setPos(short(x()) - x_rem + 25, y());

    if (y_rem < 13) setPos(x(), short(y()) - y_rem);
    else setPos(x(), short(y()) - y_rem + 25);
}

void Ghost::scared_ghost() {

    state = 1;
    fit_tile();
    pixels = 2.5;
    scare_timer->start(7000);

    //Reflejamos el target con respecto al punto
    //medio del laberinto.

    x_tar = 2*x_maze + 450 - x_tar;
    y_tar = 2*y_maze + 500 - y_tar;
}

void Ghost::go_home() {

    state = 2;
    fit_tile();
    pixels = 6.25;
    scare_timer->stop();

    //Apuntamos el target hacia la puerta
    //de la casa de los fantasmas.

    x_tar = x_maze + 225;
    y_tar = y_maze + 175;
}

void Ghost::normal_ghost() {
    state = 0;
    fit_tile();
    pixels = 5;
    scare_timer->stop();
}
