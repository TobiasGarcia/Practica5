#include "pacman.h"
#include <QDebug>

Pacman::Pacman() {

    scene = new QGraphicsScene;

    scene->setSceneRect(0, 0, 800, 600);
    setScene(scene);

    setFixedSize(800, 600);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    num = make_maze(162, 10);

    player = new Player(249, 247);
    scene->addItem(player);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
}

Pacman::~Pacman() {
    delete player;
    for (short j = 0; j < num; j++) delete maze.at(j);
    delete scene;
}

short Pacman::make_maze(short x, short y) {

    bool maze_bool[21][19] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                              {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                              {1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
                              {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                              {1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
                              {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
                              {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1},
                              {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                              {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1},
                              {0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
                              {1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1},
                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                              {1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
                              {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
                              {1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1},
                              {1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                              {1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
                              {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                              {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

    short num = 0;
    for (short i = 0; i < 21; i++) {
        for (short j = 0; j < 19; j++) {
            if (maze_bool[i][j]) {
                maze.at(num) = new Wall(x + 25*j, y + 25*i);
                scene->addItem(maze.at(num++));
            }
        }
    }
    maze.at(num) = new Wall(x - 25, y + 200);
    scene->addItem(maze.at(num++));
    maze.at(num) = new Wall(x - 50, y + 225);
    scene->addItem(maze.at(num++));
    maze.at(num) = new Wall(x - 25, y + 250);
    scene->addItem(maze.at(num++));

    maze.at(num) = new Wall(x + 475, y + 200);
    scene->addItem(maze.at(num++));
    maze.at(num) = new Wall(x + 500, y + 225);
    scene->addItem(maze.at(num++));
    maze.at(num) = new Wall(x + 475, y + 250);
    scene->addItem(maze.at(num++));

    return num;
}























































