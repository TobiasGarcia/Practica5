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

    ghost = new Ghost(x_maze, y_maze, eyes, scared_ghost);

    addItem(score);
    addItem(ghost);
    addItem(player);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    connect(player, &Player::earn_point, score, &Score::increase_score);
    connect(player, &Player::new_target, ghost, &Ghost::update_target);
    connect(player, &Player::scare_ghosts, ghost, &Ghost::scare);
}

Pacman::~Pacman() {
    delete player;
    delete score;
    delete ghost;
    delete[] eyes;
    delete[] scared_ghost;
}

void Pacman::make_maze(short x_maze, short y_maze) {

    short maze_info[21][19] = {{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
                               {3, 1, 1, 1, 1, 1, 1, 1, 1, 3},
                               {3, 2, 3, 3, 1, 3, 3, 3, 1, 3},
                               {3, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                               {3, 1, 3, 3, 1, 3, 1, 3, 3, 3},
                               {3, 1, 1, 1, 1, 3, 1, 1, 1, 3},
                               {3, 3, 3, 3, 1, 3, 3, 3, 0, 3},
                               {0, 0, 0, 3, 1, 3, 0, 0, 0, 0},
                               {3, 3, 3, 3, 1, 3, 0, 3, 3, 3},
                               {0, 0, 0, 0, 1, 0, 0, 3, 0, 0},
                               {3, 3, 3, 3, 1, 3, 0, 3, 3, 3},
                               {0, 0, 0, 3, 1, 3, 0, 0, 0, 0},
                               {3, 3, 3, 3, 1, 3, 0, 3, 3, 3},
                               {3, 1, 1, 1, 1, 1, 1, 1, 1, 3},
                               {3, 1, 3, 3, 1, 3, 3, 3, 1, 3},
                               {3, 2, 1, 3, 1, 1, 1, 1, 1, 0},
                               {3, 3, 1, 3, 1, 3, 1, 3, 3, 3},
                               {3, 1, 1, 1, 1, 3, 1, 1, 1, 3},
                               {3, 1, 3, 3, 3, 3, 3, 3, 1, 3},
                               {3, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                               {3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};

    for (short i = 0; i < 21; i++) {
        for (short j = 0; j < 10; j++) {
            if (maze_info[i][j] == 1) {
                addItem(new Point(x_maze + 11 + 25*j, y_maze + 11 + 25*i));
                if (j != 9) addItem(new Point(x_maze + 461 - 25*j, y_maze + 11 + 25*i));
            }
            else if (maze_info[i][j] == 2) {
                addItem(new Point(x_maze + 11 + 25*j, y_maze + 11 + 25*i, 2));
                addItem(new Point(x_maze + 461 - 25*j, y_maze + 11 + 25*i, 2));
            }
            else if (maze_info[i][j] == 3) {
                addItem(new Wall(x_maze + 25*j, y_maze + 25*i));
                if (j != 9) addItem(new Wall(x_maze + 450 - 25*j, y_maze + 25*i));
            }
        }
    }

    addItem(new Wall(x_maze - 25, y_maze + 200));
    addItem(new Wall(x_maze - 50, y_maze + 225));
    addItem(new Wall(x_maze - 25, y_maze + 250));

    addItem(new Wall(x_maze + 475, y_maze + 200));
    addItem(new Wall(x_maze + 500, y_maze + 225));
    addItem(new Wall(x_maze + 475, y_maze + 250));
}























































