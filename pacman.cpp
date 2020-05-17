#include "pacman.h"
#include <QDebug>

Pacman::Pacman() {

    scene = new QGraphicsScene;

    scene->setSceneRect(0, 0, 800, 600);
    setScene(scene);

    setFixedSize(800, 600);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    wall1 = new Wall(150, 300);
    scene->addItem(wall1);

    wall2 = new Wall(150, 500);
    scene->addItem(wall2);

    wall3 = new Wall(350, 300);
    scene->addItem(wall3);

    wall4 = new Wall(350, 400);
    scene->addItem(wall4);

    wall5 = new Wall(350, 500);
    scene->addItem(wall5);

    player = new Player(250, 300);
    scene->addItem(player);

    player->setFlag(QGraphicsItem::ItemIsFocusable);
    player->setFocus();
}

Pacman::~Pacman() {
    delete player;
    delete wall1;
    delete wall2;
    delete scene;
}
