#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsScene>
#include <QSoundEffect>
#include "general.h"
#include "player.h"
#include "wall.h"
#include "point.h"
#include "score.h"
#include "ghost.h"
#include "message.h"

//Ésta clase modela el juego como tal.

class Pacman: public QGraphicsScene {
private:

    Score *score;
    Player *player;
    Message *message;
    short lifes_left;
    bool delete_bool;
    QGraphicsPixmapItem **lifes;
    Ghost *blinky, *pinky, *inky, *clyde;
    QPixmap *eyes, *scared_ghost, *lifes_scrpit;
    QSoundEffect *lose_sound, *pop_sound, *scare_sound, *begin_sound;

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
