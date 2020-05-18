#include "wall.h"
#include <typeinfo>
#include <QDebug>

Wall::Wall(short x, short y) {

    setRect(0, 0, width, height);
    setPos(x, y);
}

Wall::~Wall() {

    qDebug() << "HOLA";
}
