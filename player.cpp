#include "player.h"

//-----------------------------------------------------------------------------------------
//-----------------------------------------GENERAL-----------------------------------------
//-----------------------------------------------------------------------------------------

QRectF Player::boundingRect() const {

    //Aquí sucede lo mismo que en el caso de boundingRect() de la clase Ghost,
    //pero la diferencia radica en que desde el principio, para que la imágen
    //de Pacman no se viera muy grande, sería de 21 x 21 pixeles.

    return QRectF(-1, -1, 27, 27);
}

void Player::focusOutEvent(QFocusEvent *event) {

    //Éste método es utilizado para que si se minimiza la pestaña, o se clickea en
    //cualquier otra parte de la escena no se pierda el focus del item Pacman y
    //cuando se presionen las teclas sean procesadas correctamente.

    Q_UNUSED(event);
    setFocus();
}

void Player::initialize() {

    //Ésta función se ejecuta cuando se crea al jugador con new, es decir, en el
    //constructor, o cuando se vuelve a colocar al jugador en el punto de inicio
    //luego de que haya sido tocado por un fantasma. Su propósito es reiniciar
    //algunas variables que poseen valores especificos cuando se comienza el
    //juego.

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

    //Reservamos memoria para posteriormente cargar imágenes y sonidos.

    //Estos son los puntos (grandes y pequeños) que debe recoger Pacman para ganar el juego,
    //por lo cual, para que fuese más fácil estudiar el comportamiento cuando se gana el
    //juego, para no tener que recoger todos los puntos, basta colocar esta variable
    //en un número menor, como 5 por ejemplo.

    points_left = 150; //150

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

    //Éste slot procesa las teclas presionadas por el usuario.

    //Notemos que si se presiona una tecla e is_playing se encuentra en false,
    //emitimos la señal para comenzar el juego.

    if (!is_playing) {

        is_playing = true;
        emit begin();

        ghosts_walk_sound->setLoopCount(QSoundEffect::Infinite);
        ghosts_walk_sound->play();

        return;
    }

    //En el arreglo de bools pressed_dir se colocará en true las teclas que el jugador éste presionando
    //actualmente, mientras que en last_presesed se almacenará la última tecla presionada; esto es con
    //el propósito de poder hacer que cuando el jugador se mueva en una dirección, por ejemplo a la
    //derecha, y mantenga presionada al mismo tiempo la tecla para ir en otra dirección, hacia
    //arriba por ejemplo, Pacman tomará el primer giro hacía arriba que encuentre; éste hecho
    //también es cierto en las demás direcciones.

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

    //Éste slot procesa las teclas que son soltadas por el usuario.

    if ((event->key() == Qt::Key_Up) and !event->isAutoRepeat()) pressed_dir.at(0) = false;
    if ((event->key() == Qt::Key_Left) and !event->isAutoRepeat()) pressed_dir.at(1) = false;
    if ((event->key() == Qt::Key_Down) and !event->isAutoRepeat()) pressed_dir.at(2) = false;
    if ((event->key() == Qt::Key_Right) and !event->isAutoRepeat()) pressed_dir.at(3) = false;
}

void Player::stop(short x_wall, short y_wall) {

    //La forma en que se bloquea el movimiento según las colisiones consiste en que si
    //la intersección entre el objeto (jugador o fantasma) y la pared es un único pixel,
    //es decir, la intersección se dio en una de las esquina (en las Os de la figura),
    //el movimiento no se bloquea en ninguna dirección; por otra parte, si la
    //intersección es de más de un pixel, es decir, hay una parte que
    //intersecta uno de los ■s de la figura, el movimiento se bloquea
    //dependiendo del lado por donde se presentó la colisión.

    //O ■ ■ ■ ■ ■ ■ O
    //■             ■
    //■             ■
    //■             ■
    //■             ■
    //■             ■
    //■             ■
    //O ■ ■ ■ ■ ■ ■ O

    //NOTA: Ésta sólo es una figura ilustrativa, en realidad los fantasmas y el jugador
    //poseen dimensiones de 29 x 29 pixeles a los ojos de las colisiones de Qt.

    //Las variables x_left, x_right, y_left e y_right son calculadas de esta manera
    //para que cubran un rectangulo mayor que sólo los pixeles de la pared.
    //Ésto es realizado con el propósito de compensar el hecho de que
    //la posición del jugador o del fantasma es representada por el
    //pixel que se encuentra en la esquina superior izquierda de
    //su figura.

    //Colisión con paredes de 26 x 26 pixeles a los ojos de las colisiones de Qt.

    short x_left = (x_wall - 25), x_right = (x_wall + 25),
            y_up = (y_wall - 25), y_down = (y_wall + 25);

    //Es fácil ver que en la implementación de éste método para la clase Ghost se lleva
    //a cabo la idea anteriormente mencionada, la que es explica justo antes de la figura
    //ilustrativa de la pared, sin embargo, ésta idea causaba que el hecho de girar en una
    //intersección fuera muy complicado para el jugador, pues al ser el movimiento tan preciso,
    //para poder girar había que colocarse en el pixel exacto y en caso de no aplicar la opción
    //mencionada dentro del método keyPressEvent(), se podía tornar verdaderamente difícil el
    //girar por una intersección, por lo cual, para resolver éste problema decidí agreagar
    //algo así como un "suavizador" de esquinas.

    //Ésto de suavizar las esquinas simplemetne consiste en que el movimiento es bloqueado de forma
    //similar a como se explicó antes, si la intersección era por una esquina no se bloqueba en ninguna
    //dirección, pero ahora se diseñó para que si la intersección se realiza por unos cuantos pixeles
    //cerca de la esquina, 14 pixeles más específicamente, en lugar de bloquear el movimiento se llama
    //al método fit_tile() para que coloqué a Pacman en el pixel exacto para poder girar con facilidad.

    //Una representación de la idea anteriormente mencionada podría ser la siguiente:

    //O % % ■ ■ % % O
    //%             %
    //%             %
    //■             ■
    //■             ■
    //%             %
    //%             %
    //O % % ■ ■ % % O

    //Donde si la intersección se da en una de las Os el movimiento no se bloquea, si la intersección
    //toca alguno de los ■s el movimiento se bloquea en la dirección por donde se presentó la colisión,
    //y si la colisión toca alguno de los %s Pacman es colocado en la baldosa adecuada para que pueda girar.

    //NOTA 1: De nuevo la figura sólo es ilustrativa.
    //NOTA 2: Ésta sólo es una explicación básica del procedimiento utilizado para "suavizar" las esquinas,
    //en realidad habían varios problemas que presentaba el implementar éste método así como es explicada;
    //naturalmente resolví éstos problemas, sin embargo, la forma final que tomó el algoritmo que rige el
    //movimiento del jugador, parece un poco extraña a primera vista, pero esto es debido a que es el
    //resultado de la toma de múltiples decisiones de diseño respecto a los problemas que se iban
    //presentando durante la implementación, por lo cual se me hace difícil el explicar aquí todas
    //esas decisiones, no obstante la escensia tras éste algoritmo es plasmada en la explicación
    //básica que se brindó hace un momento.

    if (y() == y_up) {
        need_fit.at(1) = false;
        need_fit.at(3) = false;
        if (((x_left + 14) < x()) and (x() < (x_right - 14))) {
            move_dir.at(2) = false;
            need_fit.at(2) = false;
        }
    }
    if (y() == y_down) {
        need_fit.at(1) = false;
        need_fit.at(3) = false;
        if (((x_left + 14) < x()) and (x() < (x_right - 14))) {
            move_dir.at(0) = false;
            need_fit.at(0) = false;
        }
    }

    if (x() == x_left) {
        need_fit.at(0) = false;
        need_fit.at(2) = false;
        if (((y_up + 14) < y()) and (y() < (y_down - 14))) {
            move_dir.at(3) = false;
            need_fit.at(3) = false;
        }
    }
    if (x() == x_right) {
        need_fit.at(0) = false;
        need_fit.at(2) = false;
        if (((y_up + 14) < y()) and (y() < (y_down - 14))) {
            move_dir.at(1) = false;
            need_fit.at(1) = false;
        }
    }
}

void Player::fit_tile() {

    //Éste método lo que hace es que coloca a Pacman en una baldosa, es decir,
    //aproxima su posición en x al multiplo de 25 más cercano (porque las baldosas
    //son de 25 x 25) pero antes de hacerlo le resta la posición en x del laberinto;
    //es análogo en el caso de y.

    short x_rem = (short(x()) - X_MAZE)%25, y_rem = (short(y()) - Y_MAZE)%25;

    if (x_rem < 13) setPos(short(x()) - x_rem, y());
    else setPos(short(x()) - x_rem + 25, y());

    if (y_rem < 13) setPos(x(), short(y()) - y_rem);
    else setPos(x(), short(y()) - y_rem + 25);
}

void Player::move() {

    //Éste método rige el movimiento del jugador basandose en las teclas que tiene presionadas, la última
    //que presionó, y la posición respecto a los demás items de la escena, como los puntos, paredes y fantasmas.

    //Si freeze es true nos limitamos a retornar.

    if (freeze) return;

    //Copiamos las teclas que el usuario está presionando en los arreglos need_fit y move_dir.

    //NOTA: El arreglo de bools need_fit es utilizado para implementar el algoritmo de movimiento que se mencionó
    //anteriormente. Si el jugador desea moverse en una dirección y ésta está en true dentro de need_fit,
    //significa que en lugar de moverse, Pacman será colocado en la baldosa adecuada para que pueda
    //girar sin problemas en la dirección deseada por el usuario.

    need_fit = pressed_dir;
    move_dir = pressed_dir;

    collisions = collidingItems(Qt::IntersectsItemBoundingRect);
    for (short i = 0; i < collisions.size(); i++) {

        //La sentencia del siguiente condicional funciona de la misma forma que la siguiente:

        //if (typeid(*(collisions[i])) == typeid(Wall)) stop(collisions[i]->x(), collisions[i]->y());

        //Pero ésta última provoca una advertencia por parte de Qt, por lo cual
        //es preferible utilizar la variable intermedia item.

        QGraphicsItem *item = collisions.at(i);
        if (typeid(*item) == typeid(Wall)) stop(collisions.at(i)->x(), collisions.at(i)->y());
        else if (typeid(*item) == typeid(Point)) {

            //La línea:

            //Point *point = dynamic_cast<Point*>(item);

            //Es utilizada para poder bajar en la jerarquía de clases,
            //en éste caso bajar de QGraphicsItem* a Point*.

            Point *point = dynamic_cast<Point*>(item);

            waka_sound->play();

            //Los puntos pequeños equivalen a 10 puntos en el marcador, mientras que los puntos
            //grandes equivalen a 50.

            if (point->get_type() == 1) emit earn_point(10);
            else {
                emit earn_point(50);

                //Si Pacman come un punto grande, detenemos el sonido de los fantasmas,
                //comenzamos con el de los fantasmas asustados, y emitimos la señal
                //correspondiente.

                ghosts_walk_sound->setLoopCount(0);

                scare_sound->setLoopCount(QSoundEffect::Infinite);
                scare_sound->play();

                emit scare_ghosts();
            }

            scene()->removeItem(collisions.at(i));
            delete collisions.at(i);

            points_left--;
            if (points_left == 0) {

                //Si no restan puntos en el laberinto, es porque el jugador a ganado el juego, luego
                //detenemos todos los sonidos que se estén reproduciendo y emitimos la señal para
                //ganar el juego.

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

                //Si Pacman toca un fantasma que se encuentra asustado, gana 200 puntos.

                emit earn_point(200);

                //La variable eaten_id almacena la id del último fantasma que se comió Pacman,
                //esto es para que no se detenga el sonido de los ojos volviendo a la casa de
                //los fantasmas hasta que llegue el último fantasma comido.

                eaten_id = ghost->get_id();
                eat_ghost_sound->play();

                scare_sound->setLoopCount(0);
                if (!(only_eyes_sound->isPlaying())) {
                    only_eyes_sound->setLoopCount(QSoundEffect::Infinite);
                    only_eyes_sound->play();
                }

                ghost->go_home();
            }

            //En caso de que el fantasma éste en su estado normal, detenemos todos los sonidos
            //y emitimos la señal que ejecuta el slot to_lose(), ya que el jugador ha perdido
            //una vida.

            else if (ghost->get_state() == 0) {

                scare_sound->setLoopCount(0);
                only_eyes_sound->setLoopCount(0);
                ghosts_walk_sound->setLoopCount(0);

                emit touched_ghost();
                return;
            }
        }
    }

    //Luego de procesar las colisiones, basándonos en la información obtenida decidimos si Pacman debe
    //moverse o no, y en caso afirmativo si llamar al método fit_tile() o en qué direcció mover a Pacman.

    //NOTA: La línea num_script = (num_script + 1)%3 debe ir dentro de los condicionales para que sólo suceda
    //cuando hay movimiento, si lo dejamos por fuera siempre sucedería, aunque no sería un problema pues como
    //no hay movimiento, el método paint() no se ejecutaría y el pacman no cambiaria de script cuando
    //permaneciese estático, sin embargo, me parece que dejarlo por fuera de los condicionles, aunque
    //no haga una diferencia aparente, es un error lógico, por lo cual lo decidí dejar dentro.

    if (move_dir.at(last_presesed)) {

        //Primero verificamos si nos podemos mover en la última dirección presionada.

        if (need_fit.at(last_presesed)) fit_tile();
        else setPos(x() + pixels*gap[last_presesed], y() + pixels*gap[(last_presesed + 1)%4]);

        num_script = (num_script + 1)%3;
        dir = last_presesed;
    }

    //O en caso contrario procedemos a verificar en cual de las 4 direcciones se encuentra primero
    //un true, esto precedencia entre las direcciones sólo es tenida en cuenta al presionar varias
    //direcciones justo estando en el pixel exacto para poder girar en varias direcciones en una
    //intersección, lo cual no es común que suceda, por lo tanto esa prevalencia por defecto de
    //las direcciones casi nunca es tenida en cuenta.

    else if (move_dir.at(0)) {

        if (need_fit.at(0)) fit_tile();
        else setPos(x(), y() - pixels);

        num_script = (num_script + 1)%3;
        dir = 0;
    }
    else if (move_dir.at(1)) {

        if (need_fit.at(1)) fit_tile();
        else setPos(x() - pixels, y());

        num_script = (num_script + 1)%3;
        dir = 1;
    }
    else if (move_dir.at(2)) {

        if (need_fit.at(2)) fit_tile();
        else setPos(x(), y() + pixels);

        num_script = (num_script + 1)%3;
        dir = 2;
    }
    else if (move_dir.at(3)) {

        if (need_fit.at(3)) fit_tile();
        else setPos(x() + pixels, y());

        num_script = (num_script + 1)%3;
        dir = 3;
    }

    //Esto es del teleport de los costados.

    if ((x() == (X_MAZE + 475)) and (y() == (Y_MAZE + 225)) and !tp) {
        setPos(X_MAZE - 25, y());
        tp = true;
    }
    else if ((x() == (X_MAZE - 25)) and (y() == (Y_MAZE + 225)) and !tp) {
        setPos(X_MAZE + 475, y());
        tp = true;
    }

    //Finalmente, emitimos la señal new_target() para actualizar el target de los fantasmas
    //según las coordenadas de Pacman y la dirección en que se desea mover el jugador,
    //es decir, last_presesed.

    emit new_target(x(), y(), last_presesed);
}

//-----------------------------------------------------------------------------------------
//---------------------------------IMÁGENES Y ANIMACIONES----------------------------------
//-----------------------------------------------------------------------------------------

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {

    //Dibujamos el script correspondiente según num_script.

    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->translate(13, 13);
    painter->rotate(-dir*90);
    painter->translate(-13, -13);
    painter->drawPixmap(2, 2, script[num_script]);
}

void Player::win_animation() {

    //Ejecutamos la animación que se presenta al ganar el juego.

    dir = 3;
    num_script = 1; update(); delay(150);

    for (short i = 0; i < 3; i++) {
        eat_ghost_sound->play();
        for (num_script = 0; num_script < 3; num_script++) {
            update();
            delay(150);
        }
    }

    num_script = 1;
    update();
}

void Player::lose_animation() {

    //Ejecutamos la animación de derrota del jugador.

    //NOTA: Si el juego es cerrado en el momento en que transcurre un delay, los sonidos continuaran
    //reproduciéndoce hasta que éste termine, pues el destructor está esperando que termine para
    //proceder a librar la memoria empleada para almacenar los sonidos, por lo cual cuando se
    //cierra el juego mientras se reproduce la animación de derrota del jugador, el sonido de
    //la animación continuará reproduciéndoce hasta que termine, incluso así el juego se
    //encuentre cerrado.

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

    //Éste método es sólo para saber cuando detener los sonidos de scare_sound u only_eyes_sound,
    //y volver a reproducir el sondio ghosts_walk_sound.

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
