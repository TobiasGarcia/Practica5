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
    QGraphicsPixmapItem *block1, *block2;
    Player *player;
    Score *score;
    QPixmap *eyes, *scared_ghost;
    Ghost *blinky, *pinky, *inky, *clyde;

    void make_maze(short x, short y);

public:
    Pacman(short width_game, short height_game);
    ~Pacman();
};

#endif // PACMAN_H
