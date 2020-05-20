#include "pacman.h"
#include <QDebug>

Pacman::Pacman(short width_game, short height_game) {

    setSceneRect(0, 0, width_game, height_game);
    make_maze(x_maze, y_maze);

    addItem(score);
    addItem(ghost);
    addItem(player);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();

    connect(player, &Player::earn_point, score, &Score::increase_score);
    connect(player, &Player::new_target, ghost, &Ghost::update_target);
    connect(player, &Player::scare_ghosts, ghost, &Ghost::scared_ghost);
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























































