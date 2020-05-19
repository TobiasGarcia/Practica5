#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsScene>
#include "player.h"
#include "wall.h"
#include "point.h"

class Pacman: public QGraphicsScene {
private:
    Player *player = new Player(249, 247);
public:
    Pacman();
    ~Pacman() {delete player;};
    void make_maze(short x, short y);
};

#endif // PACMAN_H
