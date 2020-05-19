#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsScene>
#include "player.h"
#include "wall.h"
#include "point.h"

class Pacman: public QGraphicsScene {
private:
    short x_maze = 60, y_maze = 50;
    Player *player = new Player(x_maze, y_maze);
public:
    Pacman();
    ~Pacman() {delete player;};
    void make_maze(short x, short y);
};

#endif // PACMAN_H
