#include "general.h"

void delay(short mili) {
    QTime sleep_time = QTime::currentTime().addMSecs(mili);
    while (QTime::currentTime() < sleep_time) QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

float calculate_dist(short x, short y, short x_tar, short y_tar)  {

    //Calculamos la distancia entre los puntos (x, y) y (x_tar, y_tar)
    //mediante la norma Euclidiana.

    return (sqrt(pow(y_tar - y, 2) + pow(x_tar - x, 2)));
}
