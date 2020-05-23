#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsScene>
#include "player.h"
#include "wall.h"
#include "point.h"
#include "score.h"
#include "ghost.h"
#include "message.h"

class Pacman: public QGraphicsScene {
private:

    bool delete_bool;
    short x_maze = 60, y_maze = 60;
    QGraphicsPixmapItem *block1, *block2;
    Player *player;
    Score *score;
    QPixmap *eyes, *scared_ghost;
    Ghost *blinky, *pinky, *inky, *clyde;
    Message *message;

    short points_left = 5;//150

    void make_maze(short x, short y);
    void create_characters();
    void remove_characters();
    void set_freeze(bool freeze);
    void initilize_characters();
    void add_characters();
    void restart_game();
    void setup_game();

public:
    Pacman(short width_game, short height_game);
    ~Pacman();

public slots:
    void to_lose();
    void to_win();
    void begin_game();

};

#endif // PACMAN_H
