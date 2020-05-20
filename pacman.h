#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsScene>
#include "player.h"
#include "wall.h"
#include "point.h"
#include "score.h"
#include "ghost.h"

class Pacman: public QGraphicsScene {
private:
    short x_maze = 60, y_maze = 60;
    Player *player;
    Score *score;
    QPixmap *eyes, *scared_ghost;
    Ghost *ghost;

    void make_maze(short x, short y);

public:
    Pacman(short width_game, short height_game);
    ~Pacman();
};

#endif // PACMAN_H
