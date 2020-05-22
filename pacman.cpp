#include "pacman.h"
#include <QDebug>

Pacman::Pacman(short width_game, short height_game) {

    setSceneRect(0, 0, width_game, height_game);
    make_maze(x_maze, y_maze);

    player = new Player(x_maze, y_maze);
    score = new Score();

    eyes = new QPixmap[4];
    eyes[0] = QPixmap(":/images/resources/images/ghosts/eyesU.png");
    eyes[1] = QPixmap(":/images/resources/images/ghosts/eyesL.png");
    eyes[2] = QPixmap(":/images/resources/images/ghosts/eyesD.png");
    eyes[3] = QPixmap(":/images/resources/images/ghosts/eyesR.png");

    scared_ghost = new QPixmap[2];
    scared_ghost[0] = QPixmap(":/images/resources/images/ghosts/scared1.png");
    scared_ghost[1] = QPixmap(":/images/resources/images/ghosts/scared2.png");

    blinky = new Ghost(x_maze, y_maze, eyes, scared_ghost, 0);
    pinky = new Ghost(x_maze, y_maze, eyes, scared_ghost, 1);
    inky = new Ghost(x_maze, y_maze, eyes, scared_ghost, 2);
    clyde = new Ghost(x_maze, y_maze, eyes, scared_ghost, 3);

    addItem(score);
    addItem(player);

    addItem(blinky);
    addItem(pinky);
    addItem(inky);
    addItem(clyde);

    //addItem(blinky->target);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //addItem(pinky->target);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //addItem(inky->target);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
    //addItem(clyde->target);//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    block1 = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/walls/empty.png"));
    block1->setPos(x_maze - 25, y_maze + 225);
    addItem(block1);

    block2 = new QGraphicsPixmapItem(QPixmap(":/images/resources/images/walls/empty.png"));
    block2->setPos(x_maze + 475, y_maze + 225);
    addItem(block2);

    connect(player, &Player::earn_point, score, &Score::increase_score);

    connect(player, &Player::new_target, blinky, &Ghost::update_target);
    connect(player, &Player::new_target, pinky, &Ghost::update_target);
    connect(player, &Player::new_target, inky, &Ghost::update_target);
    connect(player, &Player::new_target, clyde, &Ghost::update_target);

    connect(player, &Player::scare_ghosts, blinky, &Ghost::scare);
    connect(player, &Player::scare_ghosts, pinky, &Ghost::scare);
    connect(player, &Player::scare_ghosts, inky, &Ghost::scare);
    connect(player, &Player::scare_ghosts, clyde, &Ghost::scare);

    connect(blinky, &Ghost::blinky_pos, inky, &Ghost::inky_target);
}

Pacman::~Pacman() {
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
}

void Pacman::make_maze(short x_maze, short y_maze) {

    qDebug() << "NOTHING";
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























































