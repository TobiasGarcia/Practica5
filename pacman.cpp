#include "pacman.h"

Pacman::Pacman(short width_game, short height_game) {

    setSceneRect(0, 0, width_game, height_game);

    //Notemos que todas las lineas del método setup_game() podrían haber sido colocadas aquí,
    //pero no es una buena idea pues como podemos ver en pacman.h, la clase Pacman es una
    //herencia de QGraphicsScene, por lo cual cuando se termina el juego, gane o pierda
    //el jugador, limpiamos la escena con QGraphicsScene::clear() y debemos volver a
    //reservar memoria para todos los objetos, pues QGraphicsScene::clear() libera
    //la memoria utilizada, por lo cual la escena nunca es eliminada, luego es
    //más cómodo colocar todas las reservas de memoria dinámica bajo un
    //método, que en éste caso es setup_game().

    setup_game();
}

void Pacman::setup_game() {

    //Construimos el laberinto.

    make_maze(X_MAZE, Y_MAZE);

    //Cargamos los sonidos e imágenes del archivo de recursos.

    lose_sound = new QSoundEffect;
    lose_sound->setSource(QUrl("qrc:/sounds/resources/sounds/lose.wav"));

    pop_sound = new QSoundEffect;
    pop_sound->setSource(QUrl("qrc:/sounds/resources/sounds/pop.wav"));

    begin_sound = new QSoundEffect;
    begin_sound->setSource(QUrl("qrc:/sounds/resources/sounds/begin.wav"));

    eyes = new QPixmap[4];
    eyes[0] = QPixmap(":/images/resources/images/ghosts/eyesU.png");
    eyes[1] = QPixmap(":/images/resources/images/ghosts/eyesL.png");
    eyes[2] = QPixmap(":/images/resources/images/ghosts/eyesD.png");
    eyes[3] = QPixmap(":/images/resources/images/ghosts/eyesR.png");

    scared_ghost = new QPixmap[2];
    scared_ghost[0] = QPixmap(":/images/resources/images/ghosts/scared1.png");
    scared_ghost[1] = QPixmap(":/images/resources/images/ghosts/scared2.png");

    lifes_scrpit = new QPixmap[2];
    lifes_scrpit[0] = QPixmap(":/images/resources/images/pacman/pacman2.png");
    lifes_scrpit[0] = lifes_scrpit[0].transformed(QTransform().translate(-13, -13).rotate(90).translate(13, 13));
    lifes_scrpit[1] = QPixmap(":/images/resources/images/pacman/pacman14.png");

    lifes = new QGraphicsPixmapItem*[3];

    lifes[0] = new QGraphicsPixmapItem;
    lifes[0]->setPixmap(lifes_scrpit[0]);
    lifes[0]->setPos(X_MAZE + 427, Y_MAZE - 34);
    addItem(lifes[0]);

    lifes[1] = new QGraphicsPixmapItem;
    lifes[1]->setPixmap(lifes_scrpit[0]);
    lifes[1]->setPos(X_MAZE + 398, Y_MAZE - 34);
    addItem(lifes[1]);

    lifes[2] = new QGraphicsPixmapItem;
    lifes[2]->setPixmap(lifes_scrpit[0]);
    lifes[2]->setPos(X_MAZE + 369, Y_MAZE - 34);
    addItem(lifes[2]);

    lifes_left = 3;

    score = new Score;
    addItem(score);

    message = new Message;
    addItem(message);

    create_characters();

    blinky->setPos(X_MAZE + 225, Y_MAZE + 175);
    pinky->setPos(X_MAZE + 225, Y_MAZE + 225);
    inky->setPos(X_MAZE + 250, Y_MAZE + 225);
    clyde->setPos(X_MAZE + 200, Y_MAZE + 225);

#if GHOSTS_TARGETS == 1
    addItem(blinky->target);
    addItem(pinky->target);
    addItem(inky->target);
    addItem(clyde->target);
#endif

    //El propósito de ésta variable será explicado en el método restart_game().

    delete_bool = true;
}

void Pacman::create_characters() {

    //Reservamos memoria en el heap para los personajes.

    player = new Player;
    addItem(player);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    blinky = new Ghost(eyes, scared_ghost, 0);
    addItem(blinky);

    pinky = new Ghost(eyes, scared_ghost, 1);
    addItem(pinky);

    inky = new Ghost(eyes, scared_ghost, 2);
    addItem(inky);

    clyde = new Ghost(eyes, scared_ghost, 3);
    addItem(clyde);

    //Realizamos varias conexiones.

    connect(player, &Player::begin, this, &Pacman::begin_game);
    connect(player, &Player::earn_point, score, &Score::increase_score);

    connect(player, &Player::new_target, blinky, &Ghost::update_target);
    connect(player, &Player::new_target, pinky, &Ghost::update_target);
    connect(blinky, &Ghost::blinky_pos, inky, &Ghost::inky_target);
    connect(player, &Player::new_target, clyde, &Ghost::update_target);

    connect(player, &Player::scare_ghosts, blinky, &Ghost::scare);
    connect(player, &Player::scare_ghosts, pinky, &Ghost::scare);
    connect(player, &Player::scare_ghosts, inky, &Ghost::scare);
    connect(player, &Player::scare_ghosts, clyde, &Ghost::scare);

    connect(blinky, &Ghost::back_normal, player, &Player::normal_ghost);
    connect(pinky, &Ghost::back_normal, player, &Player::normal_ghost);
    connect(inky, &Ghost::back_normal, player, &Player::normal_ghost);
    connect(clyde, &Ghost::back_normal, player, &Player::normal_ghost);

    connect(player, &Player::touched_ghost, this, &Pacman::to_lose);
    connect(player, &Player::no_points_left, this, &Pacman::to_win);
}

void Pacman::make_maze(short x_maze, short y_maze) {

    //Creamos el laberinto.

    //La verdad me parece mejor implementar éste método con argumentos en lugar de escribir
    //todo con las macros X_MAZE y Y_MAZE, porque así es más fácil de adaptar a un código
    //diferente en caso de ser necesario.

    //La matriz de enteros maze_info almacena la información del laberinto en forma de enteros,
    //de tal forma que luego cuando sea recorrida, dependiendo del entero almacenado se colocará
    //un item diferente en la escena, o en el caso de las paredes, el entero representará que
    //imágen se le debe colocar.

    //0 es para no colocar nada, 1 es para colocar un punto, 2 es para colocar uno de los puntos grandes,
    //y el resto de enteros desde el 3 hasta el 27 son paredes, pero el entero difiere según la imágen
    //que deba ser colocada.

    QGraphicsPixmapItem *cover_tp;
    short maze_info[21][19] = {{ 5,  3,  3,  3,  3,  3,  3,  3,  3, 10,  3,  3,  3,  3,  3,  3,  3,  3,  6},
                               { 4,  1,  1,  1,  1,  1,  1,  1,  1, 13,  1,  1,  1,  1,  1,  1,  1,  1,  4},
                               { 4,  2, 25, 23,  1, 25, 12, 23,  1, 24,  1, 25, 12, 23,  1, 25, 23,  2,  4},
                               { 4,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  4},
                               { 4,  1, 25, 23,  1, 22,  1, 25, 12, 19, 12, 23,  1, 22,  1, 25, 23,  1,  4},
                               { 4,  1,  1,  1,  1, 13,  1,  1,  1, 13,  1,  1,  1, 13,  1,  1,  1,  1,  4},
                               { 8,  3,  3,  6,  1, 18, 12, 23,  0, 24,  0, 25, 12, 20,  1,  5,  3,  3,  7},
                               { 0,  0,  0,  4,  1, 13,  0,  0,  0,  0,  0,  0,  0, 13,  1,  4,  0,  0,  0},
                               { 3,  3,  3,  7,  1, 24,  0,  5,  3, 26,  3,  6,  0, 24,  1,  8,  3,  3,  3},
                               { 0,  0,  0,  0,  1,  0,  0,  4,  0,  0,  0,  4,  0,  0,  1,  0,  0,  0,  0},
                               { 3,  3,  3,  6,  1, 22,  0,  8,  3,  3,  3,  7,  0, 22,  1,  5,  3,  3,  3},
                               { 0,  0,  0,  4,  1, 13,  0,  0,  0,  0,  0,  0,  0, 13,  1,  4,  0,  0,  0},
                               { 5,  3,  3,  7,  1, 24,  0, 25, 12, 19, 12, 23,  0, 24,  1,  8,  3,  3,  6},
                               { 4,  1,  1,  1,  1,  1,  1,  1,  1, 13,  1,  1,  1,  1,  1,  1,  1,  1,  4},
                               { 4,  1, 25, 15,  1, 25, 12, 23,  1, 24,  1, 25, 12, 23,  1, 14, 23,  1,  4},
                               { 4,  2,  1, 13,  1,  1,  1,  1,  1,  0,  1,  1,  1,  1,  1, 13,  1,  2,  4},
                               { 9, 23,  1, 24,  1, 22,  1, 25, 12, 19, 12, 23,  1, 22,  1, 24,  1, 25, 11},
                               { 4,  1,  1,  1,  1, 13,  1,  1,  1, 13,  1,  1,  1, 13,  1,  1,  1,  1,  4},
                               { 4,  1, 25, 12, 12, 21, 12, 23,  1, 24,  1, 25, 12, 21, 12, 12, 23,  1,  4},
                               { 4,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  4},
                               { 8,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  7}};

    for (short i = 0; i < 21; i++) {
        for (short j = 0; j < 19; j++) {
            if (maze_info[i][j] == 1) addItem(new Point(x_maze + 11 + 25*j, y_maze + 11 + 25*i));
            else if (maze_info[i][j] == 2) addItem(new Point(x_maze + 11 + 25*j, y_maze + 11 + 25*i, 2));

            //Utilizamos un else if para que en el caso de que el entero sea 0 no coloque nada.

            else if (maze_info[i][j]) addItem(new Wall(x_maze + 25*j, y_maze + 25*i, maze_info[i][j]));
        }
    }

    //Añadimos paredes, pero sin imágen, alrededor de las baldosas de los costados para entrar por
    //un lado y salir por el otro con el propósito de que el jugador no se pueda salir del laberinto
    //en caso de entrar y presionar arriba o abajo.

    addItem(new Wall(x_maze - 25, y_maze + 200, 27));
    addItem(new Wall(x_maze - 50, y_maze + 225, 27));
    addItem(new Wall(x_maze - 25, y_maze + 250, 27));

    //Lo siguiente no es una pared, es sólo un cuadro negro que se coloca justo encima de la baldosa
    //donde se teleporta al jugador, para que se vea un efecto de que desaparece por un lado y aparece
    //por el otro de forma continua en lugar de súbita.

    cover_tp = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/walls/empty.png"));
    cover_tp->setPos(x_maze - 25, y_maze + 225);
    cover_tp->setZValue(1);
    addItem(cover_tp);

    //Las siguientes lineas hacen el análogo pero para el lado opuesto; pensé en colocarlo en un
    //ciclo, pero no me pareció necesario pues sólo serian dos iteraciones.

    addItem(new Wall(x_maze + 475, y_maze + 200, 27));
    addItem(new Wall(x_maze + 500, y_maze + 225, 27));
    addItem(new Wall(x_maze + 475, y_maze + 250, 27));

    cover_tp = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/walls/empty.png"));
    cover_tp->setPos(x_maze + 475, y_maze + 225);
    cover_tp->setZValue(1);
    addItem(cover_tp);

    //NOTA: No se debe liberar la memoria de cover_tp aquí porque ya no aparecería en la escena,
    //esa reserva de memoria se libera cuando se gana o se pierde, donde en cada caso la escena
    //es limpiada mediante QGraphicsScene::clear(), o cuando se cierra el juego.
}

Pacman::~Pacman() {

    //El propósito de la variable delete_bool será explicado en el método restart_game().

    if (delete_bool) {
        delete player;
        delete blinky;
        delete pinky;
        delete inky;
        delete clyde;
        delete lifes[0];
        delete lifes[1];
        delete lifes[2];
        delete[] eyes;
        delete[] lifes;
        delete[] scared_ghost;
        delete[] lifes_scrpit;
        delete score;
        delete message;
        delete lose_sound;
        delete pop_sound;
        delete begin_sound;
    }
}

void Pacman::set_freeze(bool freeze) {

    //Congelamos o descongelamos a todos los personajes.

    player->set_freeze(freeze);
    blinky->set_freeze(freeze);
    pinky->set_freeze(freeze);
    inky->set_freeze(freeze);
    clyde->set_freeze(freeze);
}

void Pacman::begin_game() {

    //Comenzamos el juego.

    blinky->setPos(X_MAZE + 175, Y_MAZE + 175);
    pinky->setPos(X_MAZE + 200, Y_MAZE + 175);
    inky->setPos(X_MAZE + 250, Y_MAZE + 175);
    clyde->setPos(X_MAZE + 275, Y_MAZE + 175);

    message->ready_msg();
    begin_sound->play();
    delay(3700);

    message->go_msg();
    delay(500);

    set_freeze(false);
    removeItem(message);
}

void Pacman::to_lose() {

    //Éste método se ejecuta cuando el jugador pierde una vida.

    set_freeze(true);
    delay(1000);

    removeItem(blinky);
    removeItem(pinky);
    removeItem(inky);
    removeItem(clyde);

    lose_sound->play();
    player->lose_animation();
    removeItem(player);

    //NOTA: Si el juego es cerrado en el momento en que transcurre un delay, los sonidos continuaran
    //reproduciéndoce hasta que éste termine, pues el destructor está esperando que termine para
    //proceder a librar la memoria empleada para almacenar los sonidos, por lo cual cuando se
    //cierra el juego mientras se reproduce la animación de derrota del jugador, el sonido de
    //la animación continuará reproduciéndoce hasta que termine, incluso así el juego se
    //encuentre cerrado.

    lifes_left--;
    delay(700);

    pop_sound->play();
    lifes[lifes_left]->setPixmap(lifes_scrpit[1]);
    delay(150);
    removeItem(lifes[lifes_left]);

    delay(1000);

    if (lifes_left == 0) {

        //En el caso en el que lifes_left es 0, significa que el jugador ya perdió
        //las tres vidas, por lo cual es un game over.

        score->final_score();

        message->game_over_msg();
        addItem(message);

        delay(5000);
        restart_game();
    }
    else {
        player->initialize();
        addItem(player);

        player->setFlag(QGraphicsItem::ItemIsFocusable);
        player->setFocus();

        blinky->initialize();
        addItem(blinky);

        pinky->initialize();
        addItem(pinky);

        inky->initialize();
        addItem(inky);

        clyde->initialize();
        addItem(clyde);

        message->press_key_msg();
        addItem(message);
    }
}

void Pacman::to_win() {

    //Éste método se ejecuta cuadno el jugador recolecta todos los puntos,
    //es decir, gana el juego.

    set_freeze(true);
    delay(1000);

    removeItem(blinky);
    removeItem(pinky);
    removeItem(inky);
    removeItem(clyde);

    player->win_animation();

    score->final_score();

    message->win_msg();
    addItem(message);

    delay(5000);
    restart_game();
}

void Pacman::restart_game() {

    //Cuando el juego se reinicia limpiamos la escena, lo cual hace que se libere toda la memoria
    //dinámica reservada por los items, por lo cual, si cerramos el juego justo en este momento,
    //el programa crashearia pues llamaria al destructor que trataria de libarar todos los
    //items que ya habían sido eliminados, sin embargo, para que esto no suceda utilziamos
    //la variable delete_bool, la cual colocamos en false hasta que se vuelva a reservar
    //toda la memoria de los items dentro de setup_game().

    QGraphicsScene::clear();
    delete_bool = false;
    setup_game();
}
