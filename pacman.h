#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsScene>
#include "general.h"
#include "player.h"
#include "wall.h"
#include "point.h"
#include "score.h"
#include "ghost.h"
#include "message.h"

class Pacman: public QGraphicsScene {
private:

    Score *score;
    Player *player;
    bool delete_bool;
    short lifes_left;
    Message *message;
    QGraphicsPixmapItem *lifes[3];
    Ghost *blinky, *pinky, *inky, *clyde;
    QPixmap *eyes, *scared_ghost, *lifes_scrpit;

    void setup_game();
    void restart_game();
    void create_characters();
    void set_freeze(bool freeze);
    void make_maze(short x, short y);

public:
    Pacman(short width_game, short height_game);
    ~Pacman();

public slots:
    void to_lose();
    void to_win();
    void begin_game();

};

#endif // PACMAN_H
