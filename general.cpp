#include "general.h"

void delay(short mili) {

    //Ésta función marca un delay de una duración de mili milisegundos. Éste código lo
    //tuve que copiar de internet porque utiliza herramientas de las cuales desconozco
    //el funcionamiento, sin embargo está hecho para generar un delay sin interrumpir el
    //procesamiento de eventos y señales, como por ejemplo las animaciones.

    QTime sleep_time = QTime::currentTime().addMSecs(mili);
    while (QTime::currentTime() < sleep_time) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

float calculate_dist(short x, short y, short x_tar, short y_tar)  {

    //Calculamos la distancia entre los puntos (x, y) y (x_tar, y_tar)
    //mediante la norma Euclidiana.

    return (sqrt(pow(y_tar - y, 2) + pow(x_tar - x, 2)));
}
