#include "ghost.h"

//-----------------------------------------------------------------------------------------
//-----------------------------------------GENERAL-----------------------------------------
//-----------------------------------------------------------------------------------------

QRectF Ghost::boundingRect() const {

    //La imágen de los fantasmas posee dimensiones de 25 x 25 pixeles, por lo cual las colisiones
    //las diseñe para rectangulos de 25 x 25, pero me di cuenta después cuando cambié de
    //QGraphicsRectItem a QGraphicsPixmapItem, que dibujar un rectangulo añadia un pixel
    //de más, en realidad eran de 26 x 26, por lo cual me tocó colocar la imágen de
    //25 x 25 pero cambiar los bordes por 26 x 26, y tres pixeles extra para que
    //hubiera una intersección entre el rectangulo del fantasma y la pared antes
    //de que el fantasma la atravesase, de esta forma Qt logra detectar la
    //colisión en el momento justo en que necesito bloquear el movimiento,
    //por lo cual al final quedo de 29 x 29 con el origen trasladado un
    //pixel en la dirección negativa de cada eje.

    return QRectF(-1, -1, 27, 27);
}

void Ghost::initialize() {

    //Ésta función está diseñada con el propósito de reiniciar el fantasma cuando el jugador
    //pierde, notemos que es mejor utilizar esto a simplemetne eliminar el fantasma y volver
    //a llamar al constructor, pues nos ahorramos volver a tener que reservar memoria para
    //las imágenes, reservar para los timers, y hacer las conexiones de las señales con los slots.

    //La variable state representa en cual estado de tres se encuentra el fantasma, los estados
    //son:

    //0: Normal, cuando el fantasma está persiguiendo a Pacman.
    //1: Asustado, cuando Pacman ha comido uno de los puntos grandes y los fantasmas se ponen
    //azules y éste puede comerlos.
    //2: Ojos, cuando el fantasma ha sido comido por Pacman y sus ojos tiene que ir hacia la
    //puerta de la casa de los fantasmas para volver a su estado normal de perseguir a Pacman.

    state = 0;

    //La variable pixels corresponde a la cantidad de pixeles que avanza el fantasma cuando
    //se mueve mediante el método move().

    pixels = 5;

    //Si la variable freeze está en true, el fantasma se mantiene estático; en caso de estar
    //en false el fantasma se mueve por el laberinto en búsqueda de su target.

    freeze = true;

    //La variable sheet_bool sólo con el propósito de cambiar la sábana del fantasma para
    //que se vea como si estuviera levitando por el laberinto.

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

    //La id respresenta a cual fantasma se refiere:

    //0: Blinky (Rojo)
    //1: Pinky (Rosa)
    //2: Inky (Azul)
    //3: Clyde (Naranja)

    //Como todos poseen los mismos ojos y la misma forma cuando están asustados,
    //es decir, cuando se ponen azul oscuro y el jugador los puede comer, éstas
    //imágenes las cargamos desde el los métodos del juego como tal, y simplemente
    //le pasamos la dirección de memoria donde están ubicadas.

    id = _id;

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

//El movimiento de los fantasmas es sencillo, y está programado como en el pacman original
//según se explica en éste vídeo entre los minutos 1:55 y 12:50:

//https://www.youtube.com/watch?v=ataGotQ7ir8&t=115s

//NOTA: Del vídeo sólo se tomaron los métodos para calcular los targets de los fantasmas
//y la forma en que llegan a estos.

//La idea consiste en ubicar un punto llamado target que será el lugar hacia donde tratará
//de ir el fantasma, digo "tratará" pues este punto puede estar por fuera del laberinto.
//Para "llegar" hasta éste target, se utiliza el método move(), el cual decide que camino
//tomar en una intersección según la posición del target; éste último método está diseñado
//para que no ocurra ningún problema en caso de que el target esté por fuera del laberinto.

//Los siguientes métodos _target() actualzan el target del fantasma dependiento de cual es,
//según lo mencionado en el vídeo.

void Ghost::blinky_target(short x_pac, short y_pac) {

    //Lo colocamos en la posición de Pacman.

    x_tar = x_pac;
    y_tar = y_pac;

#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
}

void Ghost::pinky_target(short x_pac, short y_pac, short dir_pac) {

    //Apuntamos 40 pixeles más adelante de la posición donde se encuentra
    //Pacman según la dirección en que se está dirigiendo.

    x_tar = x_pac + 40*gap[dir_pac];
    y_tar = y_pac + 40*gap[(dir_pac + 1)%4];

#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
}

void Ghost::inky_target(short x_pac, short y_pac, short x_blin, short y_blin, short dir_pac) {

    if (!state) {

        //Calculamos el target auxiliar que se encuentra 20 pixeles delante de la posición
        //de Pacman según la dirección en que éste se está dirigiendo.

        x_tar = x_pac + 20*gap[dir_pac];
        y_tar = y_pac + 20*gap[(dir_pac + 1)%4];

        //Reflejamos la posición de Blinky con respecto al target auxiliar.

        x_tar = 2*x_tar - x_blin;
        y_tar = 2*y_tar - y_blin;

#if GHOSTS_TARGETS == 1
        target->setPos(x_tar, y_tar);
#endif
    }
}

void Ghost::clyde_target(short x_pac, short y_pac) {

    //Colocamos el target sobre Pacman a menos que éste se encuentre a 80
    //pixeles o menos de Clyde, en cuyo caso colocamos el target en la
    //esquina inferior izquierda del laberinto.

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

    //Éste método es una slot que es activado por la señal new_target() emitida
    //por el jugador o por la señal blinky_pos() que es emitida por Blinky.
    //Su propósito es actualizar el target del fantasma en cuestión.

    //Notemos que si el fantasma en cuestión es Blinky, es decir, id = 0,
    //emitimos la señal para actualizar el target de Inky, ya dentro del
    //slot inky_target() decidimos si actualizar el target de Inky o no
    //dependiendo de si está en el estado 0 o no.

    //Para el resto de fantasmas primero verificamos que se encuentren en el
    //estado 0, que corresponde al estado en donde persiguen a Pacman.

    if (id == 0) emit blinky_pos(x_pac, y_pac, x(), y(), dir_pac);
    if (!state) {
        if (id == 3) clyde_target(x_pac, y_pac);
        else if (id == 1) pinky_target(x_pac, y_pac, dir_pac);
        else if (id == 0) blinky_target(x_pac, y_pac);
    }
}

void Ghost::choose_dir() {

    //Éste método determina la dirección que debe seguir el fantasma en cada paso
    //que da, según como se explica en el vídeo anteriormente mencionado.

    //La variable min es la dirección en que se moverá el fantasma, las direcciones
    //son representadas por un número entero entre el 0 y el 3, inclusive, de ésta
    //forma:

    //0: Arriba
    //1: Izquierda
    //2: Abajo
    //3: Derecha

    //NOTA: En principio no es necesario inicializar la variable min, pues no hay callejones
    //sin salida en el laberinto, pero en el caso de que se agregue alguno en otra configuración
    //para éste, con el propósito de que el fantasma se regrese por donde ingresó en caso de entrar
    //en uno de ellos, inicializamos min con la dirección opuesta a la dirección que se dirije el fantasma.

    short min = (dir + 2)%4;
    float min_dist = 2048, dist;

    for (short i = 0; i < 4; i++) {

        //No consideramos la dirección opuesta a la que se está moviendo actualmente
        //para calcular la mínima distancia.

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

    //NOTA: El arreglo de bools move_dir indica en cual dirección se debe mover el fantasma,
    //la cual es la única que esté en true.
}

void Ghost::fit_tile() {

    //Éste método lo que hace es que coloca al fantasma en una baldosa, es decir,
    //aproxima su posición en x al multiplo de 25 más cercano (porque las baldosas
    //son de 25 x 25) pero antes de hacerlo le resta la posición en x del laberinto;
    //es análogo en el caso de y.

    //Esto es porque cuando hay cambios en la magnitud de la velocidad, como por
    //ejemplo colocar el atributo pixels en 2.5, hay que asegurarnos que el fantasma
    //éste dentro de una baldosa, o de lo contrario el fantasma comenzará a atravesar
    //las paredes.

    short x_rem = (short(x()) - X_MAZE)%25, y_rem = (short(y()) - Y_MAZE)%25;

    if (x_rem < 13) setPos(short(x()) - x_rem, y());
    else setPos(short(x()) - x_rem + 25, y());

    if (y_rem < 13) setPos(x(), short(y()) - y_rem);
    else setPos(x(), short(y()) - y_rem + 25);
}

void Ghost::scare() {

    //Éste método es un slot que se activa cuando Pacman come uno de los puntos grandes;
    //lo que hace es cambiar el estado del fantasma a 1, es decir, a estar asustado,
    //reducir la magnitud de su velocidad y girar 180 grados la dirección en la que
    //se mueve, además de reflejar su target actual con respecto al centro
    //del laberinto.

    state = 1;
    fit_tile();
    pixels = 2.5;
    dir = (dir + 2)%4;
    scare_timer->start(7000);

    x_tar = 2*(X_MAZE + 225) - x_tar;
    y_tar = 2*(Y_MAZE + 250) - y_tar;

#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
}

void Ghost::go_home() {

    //Éste slot se ejecuta cuando un fantasma asustado es comido por Pacman;
    //lo que hace es colocar el estado en 2 para que sólo se muestren los
    //ojos del fantasma, aumentar la magnitud de su velocidad y colocar
    //su objetivo en la puerta de la casa de los fantasmas.

    state = 2;
    fit_tile();
    pixels = 6.25;
    scare_timer->stop();

    x_tar = X_MAZE + 225;
    y_tar = Y_MAZE + 175;

#if GHOSTS_TARGETS == 1
    target->setPos(x_tar, y_tar);
#endif
}

void Ghost::normal_ghost() {

    //Éste slot se ejecuta cuando el fantasma llega a la puerta de la casa de los fantasmas
    //en el estado 2, es decir, cuando sólo se ven sus ojos luego de ser comido por Pacman;
    //lo que haces es devolver el estado a 0, es decir, al estado normal, colcar la
    //magnitud de la velocidad de nuevo en 5 pixeles por cada 50 milisegundos
    //(100 pixeles por segundo) y emitir la seña back_normal para que la
    //procese el slot normal_ghost de la clase Player.

    state = 0;
    fit_tile();
    pixels = 5;
    scare_timer->stop();
    emit back_normal(id);
}

void Ghost::set_freeze(bool _freeze) {

    freeze = _freeze;
    if (freeze) scare_timer->stop();
}

void Ghost::stop(short x_wall, short y_wall) {

    //Colisión con paredes de 26 x 26 pixeles a los ojos de las colisiones de Qt.

    //La documentació de éste método es análoga a la del método que lleva éste mismo nombre
    //pero en la clase Player, sólo que aquí no hace falta el detalle de "suavizar" las esquinas.

    short x_left = (x_wall - 25), x_right = (x_wall + 25),
            y_up = (y_wall - 25), y_down = (y_wall + 25);

    if ((y() == y_up) and (x() != x_left) and (x() != x_right)) move_dir.at(2) = false;
    if ((y() == y_down) and (x() != x_left) and (x() != x_right)) move_dir.at(0) = false;
    if ((x() == x_left) and (y() != y_up) and (y() != y_down)) move_dir.at(3) = false;
    if ((x() == x_right) and (y() != y_up) and (y() != y_down)) move_dir.at(1) = false;
}

void Ghost::move() {

    //Éste slot se ejecuta cada 50 milisegundos y se encarga de mover al fantasma.

    //Si freeze es true no hacemos nada, sólo retornamos.

    if (freeze) return;

    //Si estamos en el estado 2 y llegamos a la puerta de la casa de los fantasmas,
    //emitimos la señal normal_ghost().

    if ((state == 2) and ((x() == (X_MAZE + 225)) and (y() == (Y_MAZE + 175)))) normal_ghost();

    //Si estamos en una de las baldosas laterales por donde se entra y se sale por el otro lado.

    if ((x() == (X_MAZE + 475)) and (y() == (Y_MAZE + 225))) setPos(X_MAZE - 25, y());
    else if ((x() == (X_MAZE - 25)) and (y() == (Y_MAZE + 225))) setPos(X_MAZE + 475, y());

    //Simpre suponemos que nos podemos mover en todas direcciones y luego filtramos en cuales
    //no hay paredes, para finalmente elegir la de menor distancia con respecto al target
    //como se explica en el vídeo anteriormente mencionado.

    move_dir.fill(true);

    //NOTA: El arreglo de bools move_dir indica en cual dirección se debe mover el fantasma,
    //la cual es la única que esté en true.

    //Filtamos para colocar en false las direcciones donde hay paredes.

    collisions = collidingItems(Qt::IntersectsItemBoundingRect);
    for (short i = 0; i < collisions.size(); i++) {

        //La sentencia del siguiente condicional funciona de la misma forma que la siguiente:

        //if (typeid(*(collisions[i])) == typeid(Wall)) stop(collisions[i]->x(), collisions[i]->y());

        //Pero ésta última provoca una advertencia por parte de Qt, por lo cual
        //es preferible utilizar la variable intermedia item.

        QGraphicsItem *item = collisions.at(i);
        if (typeid(*item) == typeid(Wall)) stop(collisions.at(i)->x(), collisions.at(i)->y());
    }

    //Luego de quedarnos solamente con las direcciones que no poseen obstáculos,
    //mediante el método choose_dir() nos quedamos con una única dirección.

    choose_dir();

    if (move_dir.at(0)) setPos(x(), y() - pixels);
    else if (move_dir.at(1)) setPos(x() - pixels, y());
    else if (move_dir.at(2)) setPos(x(), y() + pixels);
    else setPos(x() + pixels, y());

    //NOTA IMPORTANTE: Gracias a que el movimiento es tan preciso, hay veces en que un fantasma
    //se coloca encima de otro y parece que uno de los dos desapareció del laberinto, aunque
    //en realidad es que uno de ellos simplemente está tapando al otro.
}

//-----------------------------------------------------------------------------------------
//---------------------------------IMÁGENES Y ANIMACIONES----------------------------------
//-----------------------------------------------------------------------------------------

void Ghost::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (state == 0) {

        //Si el estado es 0 simplemente colocamos la sábana y los ojos según
        //la dirección en que se está moviendo el fantasma.

        painter->drawPixmap(0, 0, sheets[sheet_bool]);
        painter->drawPixmap(0, 0, eyes[dir]);
    }

    //Si el fantasma está asustado, colocamos la sábana azul oscuro y pero no los ojos.

    else if (state == 1) painter->drawPixmap(0, 0, scared_ghost[sheet_bool]);

    //Si el fantasma está en el estado 2, simplemetne dibujamos los ojos de éste según
    //la dirección en que se está moviendo.

    else painter->drawPixmap(0, 0, eyes[dir]);
}
