#include "wall.h"

QRectF Wall::boundingRect() const {
    return QRectF(0, 0, 25, 25);
}

Wall::Wall(short x, short y, short type) {

    //Éste montón de condicionales son utilizados para decidir cuál imágen colocarle a la pared de entre
    //las 25 posibles.

    //NOTA: En el archivo de recursos sólo hay 9 imágenes diferentes, el resto de las 25 se logran
    //a partir de rotaciones de 7 de éstas.

    if (type == 3) {
        image = new QPixmap(":/images/resources/images/walls/horizontal2.png");
        *image = image->transformed(QTransform().translate(-13, -13).rotate(90).translate(13, 13));
    }
    else if (type == 4) image = new QPixmap(":/images/resources/images/walls/horizontal2.png");
    else if (type <= 8) {
        image = new QPixmap(":/images/resources/images/walls/corner2.png");
        *image = image->transformed(QTransform().translate(-13, -13).rotate((type - 5)*90).translate(13, 13));
    }
    else if (type <= 11) {
        image = new QPixmap(":/images/resources/images/walls/t2.png");
        *image = image->transformed(QTransform().translate(-13, -13).rotate((type - 9)*90).translate(13, 13));
    }
    else if (type == 12) {
        image = new QPixmap(":/images/resources/images/walls/horizontal1.png");
        *image = image->transformed(QTransform().translate(-13, -13).rotate(90).translate(13, 13));
    }
    else if (type == 13) image = new QPixmap(":/images/resources/images/walls/horizontal1.png");
    else if (type <= 17) {
        image = new QPixmap(":/images/resources/images/walls/corner1.png");
        *image = image->transformed(QTransform().translate(-13, -13).rotate((type - 14)*90).translate(13, 13));
    }
    else if (type <= 21) {
        image = new QPixmap(":/images/resources/images/walls/t1.png");
        *image = image->transformed(QTransform().translate(-13, -13).rotate((type - 18)*90).translate(13, 13));
    }
    else if (type <= 25) {
        image = new QPixmap(":/images/resources/images/walls/end.png");
        *image = image->transformed(QTransform().translate(-13, -13).rotate((type - 22)*90).translate(13, 13));
    }
    else if (type == 26) image = new QPixmap(":/images/resources/images/walls/door.png");
    else image = new QPixmap(":/images/resources/images/walls/empty.png");

    setPixmap(*image);
    setPos(x, y);
}
