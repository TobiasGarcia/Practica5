#include "player.h"

//-----------------------------------------------------------------------------------------
//-----------------------------------------GENERAL-----------------------------------------
//-----------------------------------------------------------------------------------------

QRectF Player::boundingRect() const {
    return QRectF(-1, -1, 27, 27);
}

void Player::focusOutEvent(QFocusEvent *event) {
    Q_UNUSED(event);
    setFocus();
}

void Player::initialize() {

    is_playing = false;

    dir = 0;
    tp = false;
    eaten_id = 4;
    freeze = true;
    last_presesed = 0;
    normal_ghosts = 0;

    pressed_dir.fill(false);
    move_dir.fill(false);

    num_script = 0;

    setPixmap(script[0]);
    setPos(X_MAZE + 225, Y_MAZE + 375);
}

Player::Player() {

    width = 25;
    height = 25;

    points_left = 150;

    pixels = 5;

    scare_sound = new QSoundEffect;
    scare_sound->setSource(QUrl("qrc:/sounds/resources/sounds/scare.wav"));

    eat_ghost_sound = new QSoundEffect;
    eat_ghost_sound->setSource(QUrl("qrc:/sounds/resources/sounds/eat_ghost.wav"));

    only_eyes_sound = new QSoundEffect;
    only_eyes_sound->setSource(QUrl("qrc:/sounds/resources/sounds/only_eyes.wav"));

    ghosts_walk_sound = new QSoundEffect;
    ghosts_walk_sound->setSource(QUrl("qrc:/sounds/resources/sounds/ghost_walk.wav"));

    waka_sound = new QSoundEffect;
    waka_sound->setSource(QUrl("qrc:/sounds/resources/sounds/waka.wav"));

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

    move_timer = new QTimer;
    connect(move_timer, &QTimer::timeout, this, &Player::move);

    initialize();

    move_timer->start(1000/30); //30 fps
}

Player::~Player() {
    delete move_timer;
    delete[] script;
    delete scare_sound;
    delete only_eyes_sound;
    delete eat_ghost_sound;
    delete ghosts_walk_sound;
    delete waka_sound;
}

//-----------------------------------------------------------------------------------------
//---------------------------------MOVIMIENTO Y COLISIONES---------------------------------
//-----------------------------------------------------------------------------------------

void Player::keyPressEvent(QKeyEvent *event) {

    if (!is_playing) {
        is_playing = true;
        emit begin();
        ghosts_walk_sound->setLoopCount(QSoundEffect::Infinite);
        ghosts_walk_sound->play();
        return;
    }

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

void Player::stop(short x_wall, short y_wall) {

    //La forma en que se bloquea el movimiento según las colisiones consiste en que si
    //la intersección entre el objeto (jugador o fantasma) y la pared es un único pixel,
    //es decir, la intersección se dio en una de las esquina (en las Os de la figura),
    //el movimiento no se bloquea en ninguna dirección; por otra parte si la
    //intersección es de más de un pixel, es decir, hay una parte que
    //intersecta uno de los ■s de la figura, el movimiento se
    //dependiendo del lado por donde se presentó la colisión.

    //O ■ ■ ■ O
    //■       ■
    //■       ■
    //■       ■
    //O ■ ■ ■ O

    //NOTA: Ésta sólo es una figura ilustrativa, en realidad los fantasmas y el jugador
    //poseen dimensiones de 25 x 25 pixeles.

    //Las variables x_left, x_right, y_left e y_right son calculadas de esta manera
    //para que cubran un rectangulo mayor que sólo los pixeles de la pared.
    //Ésto es realizado con el propósito de compensar el hecho de que
    //la posición del jugador o del fantasma es representada por el
    //pixel que se encuentra en la esquina superior izquierda de
    //su imagen.

    //Colisión con paredes de 25 x 25 pixeles.

    short x_left = (x_wall - width), x_right = (x_wall + 25),
            y_up = (y_wall - height), y_down = (y_wall + 25);

    if ((y() == y_up) and (x() != x_left) and (x() != x_right)) move_dir.at(2) = false;
    if ((y() == y_down) and (x() != x_left) and (x() != x_right)) move_dir.at(0) = false;

    if ((x() == x_left) and (y() != y_up) and (y() != y_down)) move_dir.at(3) = false;
    if ((x() == x_right) and (y() != y_up) and (y() != y_down)) move_dir.at(1) = false;
}

void Player::move() {

    if (freeze) return;

    move_dir = pressed_dir;

    collisions = collidingItems(Qt::IntersectsItemBoundingRect);
    for (short i = 0; i < collisions.size(); i++) {

        //La sentencia del siguiente condicional funciona de la misma forma que la siguiente:

        //if (typeid(*(collisions[i])) == typeid(Wall)) stop(collisions[i]->x(), collisions[i]->y());

        //Pero ésta última provoca una advertencia por parte de Qt, por lo cual
        //es preferible utilizar la variable intermedia item.

        auto item = collisions.at(i);
        if (typeid(*item) == typeid(Wall)) stop(collisions.at(i)->x(), collisions.at(i)->y());
        else if (typeid(*item) == typeid(Point)) {

            //La línea:

            //Point *point = dynamic_cast<Point*>(item);

            //Es utilizada para poder bajar en la jerarquía de las clases,
            //en éste caso bajar de QGraphicsItem* a Point*.

            Point *point = dynamic_cast<Point*>(item);

            waka_sound->play();

            if (point->get_type() == 1) emit earn_point(10);
            else {
                emit earn_point(50);
                ghosts_walk_sound->setLoopCount(0);
                scare_sound->setLoopCount(QSoundEffect::Infinite);
                scare_sound->play();
                emit scare_ghosts();
            }

            scene()->removeItem(collisions.at(i));
            delete collisions.at(i);

            points_left--;
            if (points_left == 0) {

                scare_sound->setLoopCount(0);
                only_eyes_sound->setLoopCount(0);
                ghosts_walk_sound->setLoopCount(0);

                emit no_points_left();
                return;
            }
        }

        //La parte del < 15 es para que las colisiones con los fantasmas por parte del jugador no
        //ocurran con sólo tocarlos, sino que tengan que tocar al jugador y estar cerca de él,
        //esto es porque al momento de cambiar la dirección al girar por las esquinas,
        //si había un fantasma cerca era casi que una colisión segura; si desea puede
        //cambiar el 15 por un 30 o algo superior y verá a qué me refiero.

        else if ((typeid(*item) == typeid(Ghost)) and (calculate_dist(collisions.at(i)->x(), collisions.at(i)->y(), x(), y()) < 15)) {

            Ghost *ghost = dynamic_cast<Ghost*>(item);

            if (ghost->get_state() == 1) {
                emit earn_point(200);

                eaten_id = ghost->get_id();
                eat_ghost_sound->play();

                scare_sound->setLoopCount(0);
                if (!(only_eyes_sound->isPlaying())) {
                    only_eyes_sound->setLoopCount(QSoundEffect::Infinite);
                    only_eyes_sound->play();
                }

                ghost->go_home();
            }
            else if (ghost->get_state() == 0) {

                scare_sound->setLoopCount(0);
                only_eyes_sound->setLoopCount(0);
                ghosts_walk_sound->setLoopCount(0);

                emit touched_ghost();
                return;
            }
        }
    }

    //La línea num_script = (num_script + 1)%3 debe ir dentro de los condicionales
    //para que sólo suceda cuando hay movimiento, si lo dejamos por fuera siempre
    //sucedería, aunque no sería un problema pues como no hay movimiento el
    //método paint() no se ejecutaría y el pacman no cambiaria de script
    //cuando permaneciese estático, sin embargo me parece que dejarlo por
    //fuera de los condicionles, aunque no haga una diferencia aparente,
    //es un error lógico, por lo cual lo dejé dentro de cada uno de ellos.

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
        num_script = (num_script + 1)%3;
        dir = 3;
    }

    //NOTA: Como el movimiento es tan preciso, al momento de girar por una esquina
    //o tomar otro camino en una intersección, hay que estar en el pixel exacto
    //o de lo contrario no se podrá girar. Esto hacía que el juego fuera un
    //poco difícil, pero para solucionarlo programé que cuando se va en
    //una dirección, por ejemplo a la derecha, y se mantiene presionada
    //al mismo tiempo la tecla para ir en otra dirección, hacia arriba
    //por ejemplo, el personaje tomará el primer giro hacía arriba
    //que encuentre; el procedimiento es análogo en las
    //demás direcciones.

    //Esto es del teleport de los costados.

    if ((x() == (X_MAZE + 475)) and (y() == (Y_MAZE + 225)) and !tp) {
        setPos(X_MAZE - 25, y());
        tp = true;
    }
    else if ((x() == (X_MAZE - 25)) and (y() == (Y_MAZE + 225)) and !tp) {
        setPos(X_MAZE + 475, y());
        tp = true;
    }

    //Enviamos la dirección hacia donde quiere ir el jugador.

    emit new_target(x(), y(), last_presesed);
}

//-----------------------------------------------------------------------------------------
//----------------------------------------IMÁGENES-----------------------------------------
//-----------------------------------------------------------------------------------------

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->translate(13, 13);
    painter->rotate(-dir*90);
    painter->translate(-13, -13);
    painter->drawPixmap(2, 2, script[num_script]);
}

void Player::win_animation() {

    dir = 3;
    num_script = 1; update(); delay(150);

    for (short i = 0; i < 3; i++) {
        for (num_script = 0; num_script < 3; num_script++) {
            update();
            delay(150);
        }
    }

    num_script = 1;
    update();
}

void Player::lose_animation() {

    for (num_script = 0; num_script < 14; num_script++) {
        update();
        delay(100);
    }

    update();
    is_playing = false;
}

//-----------------------------------------------------------------------------------------
//----------------------------------------SONIDOS------------------------------------------
//-----------------------------------------------------------------------------------------

void Player::normal_ghost(short id) {

    normal_ghosts++;
    if (normal_ghosts == 4) {

        eaten_id = 4;
        normal_ghosts = 0;

        scare_sound->setLoopCount(0);
        only_eyes_sound->setLoopCount(0);

        ghosts_walk_sound->setLoopCount(QSoundEffect::Infinite);
        ghosts_walk_sound->play();
    }
    else if (eaten_id == id) {
        eaten_id = 4;
        only_eyes_sound->setLoopCount(0);
        scare_sound->setLoopCount(QSoundEffect::Infinite);
        scare_sound->play();
    }
}
