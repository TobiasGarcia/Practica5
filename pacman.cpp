#include "pacman.h"

Pacman::Pacman() {

    scene = new QGraphicsScene;

    scene->setSceneRect(0, 0, 800, 600);
    setScene(scene);

    setFixedSize(800, 600);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    player = new Player(scene->width(), scene->height());

    scene->addItem(player);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
}
