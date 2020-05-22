#include "ghost.h"

//-------------------------------------------------------------------GENERAL

QRectF Ghost::boundingRect() const {
    return QRectF(-1, -1, 27, 27);
}

Ghost::Ghost(short _x_maze, short _y_maze, QPixmap *_eyes, QPixmap *_scared_ghost, short _id) {

    x_maze = _x_maze;
    y_maze = _y_maze;

    x_tar = x_maze + 225;
    y_tar = y_maze + 375;

    id = _id;

    if (id == 0) dir = 1;
    else dir = 3;

    eyes = _eyes;
    scared_ghost = _scared_ghost;

    sheets = new QPixmap[2];
    if (id == 3) {
        sheets[0] = QPixmap(":/images/resources/images/ghosts/clyde1.png");
        sheets[1] = QPixmap(":/images/resources/images/ghosts/clyde2.png");
    }
    else if (id == 2) {
        sheets[0] = QPixmap(":/images/resources/images/ghosts/inky1.png");
        sheets[1] = QPixmap(":/images/resources/images/ghosts/inky2.png");
    }
    else if (id == 1) {
        sheets[0] = QPixmap(":/images/resources/images/ghosts/pinky1.png");
        sheets[1] = QPixmap(":/images/resources/images/ghosts/pinky2.png");
    }
    else {
        sheets[0] = QPixmap(":/images/resources/images/ghosts/blinky1.png");
        sheets[1] = QPixmap(":/images/resources/images/ghosts/blinky2.png");
    }

    setPixmap(sheets[0]);
    setPos(x_maze + 225, y_maze + 175);

    //target = new QGraphicsPixmapItem;//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //target->setPixmap(sheets[0]);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    move_timer = new QTimer;
    scare_timer = new QTimer;
    animation_timer = new QTimer;

    connect(move_timer, &QTimer::timeout, this, &Ghost::move);
    connect(animation_timer, &QTimer::timeout, this, &Ghost::animate_ghost);
    connect(scare_timer, &QTimer::timeout, this, &Ghost::normal_ghost);

    move_timer->start(1000/30);
    animation_timer->start(100);
}

Ghost::~Ghost() {
    delete move_timer;
    delete scare_timer;
    delete[] sheets;
    delete animation_timer;
    //delete target;//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
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

void Ghost::blinky_target(short x_pac, short y_pac) {

    //Lo colocamos en la posición de Pacman.

    x_tar = x_pac;
    y_tar = y_pac;
    //target->setPos(x_tar, y_tar);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
}

void Ghost::pinky_target(short x_pac, short y_pac, short dir_pac) {
    x_tar = x_pac + 40*gap[dir_pac];
    y_tar = y_pac + 40*gap[(dir_pac + 1)%4];
    //target->setPos(x_tar, y_tar);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
}

void Ghost::inky_target(short x_pac, short y_pac, short x_blin, short y_blin, short dir_pac) {

    if (!state) {

        //Calculamos el target auxiliar.

        x_tar = x_pac + 20*gap[dir_pac];
        y_tar = y_pac + 20*gap[(dir_pac + 1)%4];

        //Reflejamos la posición de Blinky con
        //respecto al target auxiliar.

        x_tar = 2*x_tar - x_blin;
        y_tar = 2*y_tar - y_blin;
        //target->setPos(x_tar, y_tar);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    }
}

void Ghost::clyde_target(short x_pac, short y_pac) {

    if (calculate_dist(x(), y(), x_pac, y_pac) > 80) {
        x_tar = x_pac;
        y_tar = y_pac;
    }
    else {
        x_tar = x_maze;
        y_tar = y_maze + 525;
    }
    //target->setPos(x_tar, y_tar);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
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

void Ghost::update_target(short x_pac, short y_pac, short dir_pac) {

    //La señal binky_pos() lo utilizamos para activar el slot
    //inky_target del fantasma Inky, pues éste última necesita
    //la posición de Blinky para poder definir su target.

    if (!state) {
        if (id == 3) clyde_target(x_pac, y_pac);
        else if (id == 1) pinky_target(x_pac, y_pac, dir_pac);
        else if (id == 0) {
            blinky_target(x_pac, y_pac);
            emit blinky_pos(x_pac, y_pac, x(), y(), dir_pac);
        }
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

void Ghost::scare() {

    state = 1;
    fit_tile();
    pixels = 2.5;
    scare_timer->start(7000);

    //Reflejamos el target con respecto al
    //centro del laberinto.

    x_tar = 2*(x_maze + 225) - x_tar;
    y_tar = 2*(y_maze + 250) - y_tar;
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

    //target->setPos(x_tar, y_tar);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
}

void Ghost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (state == 0) {
        painter->drawPixmap(0, 0, sheets[sheet_bool]);
        painter->drawPixmap(0, 0, eyes[dir]);
    }
    else if (state == 1) painter->drawPixmap(0, 0, scared_ghost[sheet_bool]);
    else painter->drawPixmap(0, 0, eyes[dir]);
}

void Ghost::normal_ghost() {
    state = 0;
    fit_tile();
    pixels = 5;
    scare_timer->stop();
}
