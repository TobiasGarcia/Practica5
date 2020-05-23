#include "pacman.h"
#include <QDebug>

Pacman::Pacman(short width_game, short height_game) {

    setSceneRect(0, 0, width_game, height_game);
    setup_game();
}

void Pacman::create_characters() {

    player = new Player(x_maze, y_maze);

    blinky = new Ghost(x_maze, y_maze, eyes, scared_ghost, 0);
    pinky = new Ghost(x_maze, y_maze, eyes, scared_ghost, 1);
    inky = new Ghost(x_maze, y_maze, eyes, scared_ghost, 2);
    clyde = new Ghost(x_maze, y_maze, eyes, scared_ghost, 3);

    block1 = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/walls/empty.png"));
    block1->setPos(x_maze - 25, y_maze + 225);

    block2 = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/walls/empty.png"));
    block2->setPos(x_maze + 475, y_maze + 225);

    add_characters();

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

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

Pacman::~Pacman() {
    if (delete_bool) {
        delete player;
        delete score;
        delete blinky;
        delete pinky;
        delete inky;
        delete clyde;
        delete[] eyes;
        delete[] scared_ghost;
        delete block1;
        delete block2;
        delete message;
    }
}

void Pacman::to_lose() {

    set_freeze(true);
    delay(1000);
    remove_characters();

    player->lose_animation();
    removeItem(player);

    delay(1000);

    initilize_characters();
    add_characters();

    message->press_key_msg();
    addItem(message);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
}

void Pacman::to_win() {

    set_freeze(true);
    delay(1000);
    remove_characters();

    player->win_animation();

    score->win_score();
    message->win_msg();
    addItem(message);

    delay(5000);
    restart_game();
}

void Pacman::begin_game() {

    blinky->setPos(x_maze + 175, y_maze + 175);
    pinky->setPos(x_maze + 200, y_maze + 175);
    inky->setPos(x_maze + 250, y_maze + 175);
    clyde->setPos(x_maze + 275, y_maze + 175);

    message->ready_msg();
    delay(1000);
    message->go_msg();
    set_freeze(false);
    delay(1000);
    removeItem(message);
}

//void Pacman::decrease_points_left(short points) {
//    if (points != 200) {
//        points_left--;
//        if (points_left == 0) win();
//    }
//}

void Pacman::make_maze(short x_maze, short y_maze) {

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

    addItem(new Wall(x_maze + 475, y_maze + 200, 27));
    addItem(new Wall(x_maze + 500, y_maze + 225, 27));
    addItem(new Wall(x_maze + 475, y_maze + 250, 27));
}

void Pacman::remove_characters() {
    removeItem(blinky);
    removeItem(pinky);
    removeItem(inky);
    removeItem(clyde);

    removeItem(block1);
    removeItem(block2);
}

void Pacman::set_freeze(bool freeze) {
    player->set_freeze(freeze);

    blinky->set_freeze(freeze);
    pinky->set_freeze(freeze);
    inky->set_freeze(freeze);
    clyde->set_freeze(freeze);
}

void Pacman::initilize_characters() {
    player->initialize();

    blinky->initialize();
    pinky->initialize();
    inky->initialize();
    clyde->initialize();
}

void Pacman::add_characters() {
    addItem(player);

    addItem(blinky);
    addItem(pinky);
    addItem(inky);
    addItem(clyde);

    addItem(block1);
    addItem(block2);
}

void Pacman::restart_game() {

    QGraphicsScene::clear();
    delete_bool = false;
    setup_game();
}

void Pacman::setup_game() {

    make_maze(x_maze, y_maze);

    eyes = new QPixmap[4];
    eyes[0] = QPixmap(":/images/resources/images/ghosts/eyesU.png");
    eyes[1] = QPixmap(":/images/resources/images/ghosts/eyesL.png");
    eyes[2] = QPixmap(":/images/resources/images/ghosts/eyesD.png");
    eyes[3] = QPixmap(":/images/resources/images/ghosts/eyesR.png");

    scared_ghost = new QPixmap[2];
    scared_ghost[0] = QPixmap(":/images/resources/images/ghosts/scared1.png");
    scared_ghost[1] = QPixmap(":/images/resources/images/ghosts/scared2.png");

    score = new Score;
    addItem(score);

    message = new Message;
    addItem(message);

    create_characters();
    set_freeze(true);

    blinky->setPos(x_maze + 225, y_maze + 175);
    pinky->setPos(x_maze + 225, y_maze + 225);
    inky->setPos(x_maze + 250, y_maze + 225);
    clyde->setPos(x_maze + 200, y_maze + 225);

    //addItem(blinky->target);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //addItem(pinky->target);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //addItem(inky->target);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //addItem(clyde->target);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    delete_bool = true;
}






















































