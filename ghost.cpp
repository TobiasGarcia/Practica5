#include "ghost.h"

//-----------------------------------------------------------------------------------------
//-----------------------------------------GENERAL-----------------------------------------
//-----------------------------------------------------------------------------------------

QRectF Ghost::boundingRect() const {
    return QRectF(-1, -1, 27, 27);
}

void Ghost::initialize() {

    state = 0;
    pixels = 5;
    freeze = true;
    sheet_bool = true;

    setPixmap(sheets[0]);

    if (id == 3) setPos(X_MAZE + 275, Y_MAZE + 175);
    else if (id == 2) setPos(X_MAZE + 250, Y_MAZE + 175);
    else if (id == 1) setPos(X_MAZE + 200, Y_MAZE + 175);
    else setPos(X_MAZE + 175, Y_MAZE + 175);

    x_tar = X_MAZE + 225;
    y_tar = Y_MAZE + 375;

#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
    target->setZValue(1);
#endif

    if ((id == 0) or (id == 1)) dir = 1;
    else dir = 3;
}

Ghost::Ghost(QPixmap *_eyes, QPixmap *_scared_ghost, short _id) {

    id = _id;

    width = 25;
    height = 25;

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

#if GHOSTS_TARGETS == 1
    target = new QGraphicsPixmapItem;
    target->setPixmap(sheets[0]);
#endif

    move_timer = new QTimer;
    scare_timer = new QTimer;
    animation_timer = new QTimer;

    connect(move_timer, &QTimer::timeout, this, &Ghost::move);
    connect(scare_timer, &QTimer::timeout, this, &Ghost::normal_ghost);
    connect(animation_timer, &QTimer::timeout, this, &Ghost::animate_ghost);

    initialize();

    move_timer->start(1000/30); //30 fps
    animation_timer->start(100);
}

Ghost::~Ghost() {
    delete move_timer;
    delete scare_timer;
    delete animation_timer;
    delete[] sheets;
#if GHOSTS_TARGETS == 1
    delete target;
#endif
}

//-----------------------------------------------------------------------------------------
//---------------------------------MOVIMIENTO Y COLISIONES---------------------------------
//-----------------------------------------------------------------------------------------

void Ghost::blinky_target(short x_pac, short y_pac) {

    //Lo colocamos en la posición de Pacman.

    x_tar = x_pac;
    y_tar = y_pac;
#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
}

void Ghost::pinky_target(short x_pac, short y_pac, short dir_pac) {
    x_tar = x_pac + 40*gap[dir_pac];
    y_tar = y_pac + 40*gap[(dir_pac + 1)%4];
#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
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
#if GHOSTS_TARGETS == 1
        target->setPos(x_tar, y_tar);
#endif
    }
}

void Ghost::clyde_target(short x_pac, short y_pac) {

    if (calculate_dist(x(), y(), x_pac, y_pac) > 80) {
        x_tar = x_pac;
        y_tar = y_pac;
    }
    else {
        x_tar = X_MAZE;
        y_tar = Y_MAZE + 525;
    }
#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
}

void Ghost::update_target(short x_pac, short y_pac, short dir_pac) {

    //La señal binky_pos() lo utilizamos para activar el slot
    //inky_target del fantasma Inky, pues éste última necesita
    //la posición de Blinky para poder definir su target.

    if (id == 0) emit blinky_pos(x_pac, y_pac, x(), y(), dir_pac);
    if (!state) {
        if (id == 3) clyde_target(x_pac, y_pac);
        else if (id == 1) pinky_target(x_pac, y_pac, dir_pac);
        else if (id == 0) blinky_target(x_pac, y_pac);
    }
}

void Ghost::choose_dir() {

    short min = (dir + 2)%4;
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

void Ghost::fit_tile() {

    short x_rem = (short(x()) - X_MAZE)%25, y_rem = (short(y()) - Y_MAZE)%25;

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

    x_tar = 2*(X_MAZE + 225) - x_tar;
    y_tar = 2*(Y_MAZE + 250) - y_tar;

#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
}

void Ghost::go_home() {

    state = 2;
    fit_tile();
    pixels = 6.25;
    scare_timer->stop();

    //Apuntamos el target hacia la puerta
    //de la casa de los fantasmas.

    x_tar = X_MAZE + 225;
    y_tar = Y_MAZE + 175;

#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
}

void Ghost::normal_ghost() {
    state = 0;
    fit_tile();
    pixels = 5;
    scare_timer->stop();
}

void Ghost::set_freeze(bool _freeze) {

    freeze = _freeze;
    if (freeze) scare_timer->stop();
}

//Este método puedo haber sido una función implementada en genearl.cpp, pero es
//mejor que no pues utiliza parametros del objeto, como width, height, x() e y().

void Ghost::stop(short x_wall, short y_wall) {

    //Colisión con paredes de 25 x 25 pixeles.

    //Para entender el funcionamiento del método ver la documentación
    //de la implementación de éste método para la clase Player.

    short x_left = (x_wall - width), x_right = (x_wall + 25),
            y_up = (y_wall - height), y_down = (y_wall + 25);

    if ((y() == y_up) and (x() != x_left) and (x() != x_right)) move_dir.at(2) = false;
    if ((y() == y_down) and (x() != x_left) and (x() != x_right)) move_dir.at(0) = false;
    if ((x() == x_left) and (y() != y_up) and (y() != y_down)) move_dir.at(3) = false;
    if ((x() == x_right) and (y() != y_up) and (y() != y_down)) move_dir.at(1) = false;
}

void Ghost::move() {

    if (freeze) return;

    if ((state == 2) and ((x() == (X_MAZE + 225)) and (y() == (Y_MAZE + 175)))) normal_ghost();

    if ((x() == (X_MAZE + 475)) and (y() == (Y_MAZE + 225))) setPos(X_MAZE - 25, y());
    else if ((x() == (X_MAZE - 25)) and (y() == (Y_MAZE + 225))) setPos(X_MAZE + 475, y());

    move_dir.fill(true);

    collisions = collidingItems(Qt::IntersectsItemBoundingRect);
    for (short i = 0; i < collisions.size(); i++) {

        //La sentencia del siguiente condicional funciona de la misma forma que la siguiente:

        //if (typeid(*(collisions[i])) == typeid(Wall)) stop(collisions[i]->x(), collisions[i]->y());

        //Pero ésta última provoca una advertencia por parte de Qt, por lo cual
        //es preferible utilizar la variable intermedia item.

        auto item = collisions.at(i);
        if (typeid(*item) == typeid(Wall)) stop(collisions.at(i)->x(), collisions.at(i)->y());
    }

    choose_dir();

    if (move_dir.at(0)) setPos(x(), y() - pixels);
    else if (move_dir.at(1)) setPos(x() - pixels, y());
    else if (move_dir.at(2)) setPos(x(), y() + pixels);
    else setPos(x() + pixels, y());

    //NOTA: Gracias a que el movimiento es tan preciso, hay veces en que un fantasma se coloca encima
    //de otro y parece que uno de los dos desapareció del laberinto, aunque en realidad es que
    //uno de ellos simplemente está tapando al otro.
}

//-----------------------------------------------------------------------------------------
//----------------------------------------IMÁGENES-----------------------------------------
//-----------------------------------------------------------------------------------------

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
