#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsView>
#include "player.h"
#include "wall.h"
#include <array>

class Pacman: public QGraphicsView {
private:
    short num;
    QGraphicsScene *scene;
    Player *player;
    std::array<Wall*, 250> maze;
public:
    Pacman();
    ~Pacman();
    short make_maze(short x, short y);
};

#endif // PACMAN_H
