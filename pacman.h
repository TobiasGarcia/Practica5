#ifndef PACMAN_H
#define PACMAN_H

#include <QGraphicsView>
#include "player.h"

class Pacman: public QGraphicsView {
private:
    QGraphicsScene *scene;
    Player *player;
public:
    Pacman();
};

#endif // PACMAN_H
