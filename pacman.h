#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsScene>
#include "player.h"
#include "wall.h"
#include "point.h"
#include "score.h"

class Pacman: public QGraphicsScene {
private:
    short x_maze = 60, y_maze = 60;
    Player *player = new Player(x_maze, y_maze);
    Score *score = new Score();
public:
    Pacman(short width_game, short height_game);
    ~Pacman() {delete player;delete score;};
    void make_maze(short x, short y);
};

#endif // PACMAN_H
