#include "pacman.h"

Pacman::Pacman(short width_game, short height_game) {

    setSceneRect(0, 0, width_game, height_game);
    setup_game();
}

void Pacman::setup_game() {

    make_maze(X_MAZE, Y_MAZE);

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

    delete_bool = true;
}

void Pacman::create_characters() {

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

    connect(player, &Player::touched_ghost, this, &Pacman::to_lose);
    connect(player, &Player::no_points_left, this, &Pacman::to_win);
}

void Pacman::make_maze(short x_maze, short y_maze) {

    //La forma en que están programados los fantasmas hace que sucedan cosas raras
    //si se ponen callejones.

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
            else if (maze_info[i][j]) addItem(new Wall(x_maze + 25*j, y_maze + 25*i, maze_info[i][j]));
        }
    }

    addItem(new Wall(x_maze - 25, y_maze + 200, 27));
    addItem(new Wall(x_maze - 50, y_maze + 225, 27));
    addItem(new Wall(x_maze - 25, y_maze + 250, 27));

    cover_tp = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/walls/empty.png"));
    cover_tp->setPos(x_maze - 25, y_maze + 225);
    cover_tp->setZValue(1);
    addItem(cover_tp);

    addItem(new Wall(x_maze + 475, y_maze + 200, 27));
    addItem(new Wall(x_maze + 500, y_maze + 225, 27));
    addItem(new Wall(x_maze + 475, y_maze + 250, 27));

    cover_tp = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/walls/empty.png"));
    cover_tp->setPos(x_maze + 475, y_maze + 225);
    cover_tp->setZValue(1);
    addItem(cover_tp);

    //No se debe liberar la memoria de cover_tp aquí porque ya no aparece en la escena,
    //esa reserva de memoria se libera cuando se cierra el juego y se elimina la escena.
}

Pacman::~Pacman() {
    if (delete_bool) {
        delete player;
        delete score;
        delete blinky;
        delete pinky;
        delete inky;
        delete clyde;
        delete[] lifes_scrpit;
        delete[] eyes;
        delete[] scared_ghost;
        delete message;

        for (short i = 0; i < 3; i++) delete lifes[i];
    }
}

void Pacman::set_freeze(bool freeze) {
    player->set_freeze(freeze);
    blinky->set_freeze(freeze);
    pinky->set_freeze(freeze);
    inky->set_freeze(freeze);
    clyde->set_freeze(freeze);
}

void Pacman::begin_game() {

    blinky->setPos(X_MAZE + 175, Y_MAZE + 175);
    pinky->setPos(X_MAZE + 200, Y_MAZE + 175);
    inky->setPos(X_MAZE + 250, Y_MAZE + 175);
    clyde->setPos(X_MAZE + 275, Y_MAZE + 175);

    message->ready_msg();
    delay(1000);

    message->go_msg();
    set_freeze(false);
    delay(1000);

    removeItem(message);
}

void Pacman::to_lose() {

    set_freeze(true);
    delay(1000);

    removeItem(blinky);
    removeItem(pinky);
    removeItem(inky);
    removeItem(clyde);

    player->lose_animation();
    removeItem(player);

    lifes_left--;
    delay(700);
    lifes[lifes_left]->setPixmap(lifes_scrpit[1]);
    delay(150);
    removeItem(lifes[lifes_left]);

    delay(1000);

    if (lifes_left == 0) {

        score->final_score();

        message->game_over_msg();
        addItem(message);

        delay(5000);
        restart_game();
        return;
    }

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

void Pacman::to_win() {

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

    QGraphicsScene::clear();
    delete_bool = false;
    setup_game();
}
