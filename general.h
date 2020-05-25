#ifndef GENERAL_H
#define GENERAL_H

#include <QCoreApplication>
#include <math.h>
#include <array>
#include <QTime>

#define GHOSTS_TARGETS 0 //Esta macro es para activar los targets de los fantasmas.
#define X_MAZE 60
#define Y_MAZE 60

void delay(short mili);
float calculate_dist(short x, short y, short x_tar, short y_tar);

#endif // GENERAL_H
