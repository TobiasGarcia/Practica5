#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsView>
#include "player.h"
#include "wall.h"

class Pacman: public QGraphicsView {
private:
    QGraphicsScene *scene;
    Player *player;
    Wall *wall1, *wall2, *wall3, *wall4, *wall5;
public:
    Pacman();
    ~Pacman();
};

#endif // PACMAN_H
