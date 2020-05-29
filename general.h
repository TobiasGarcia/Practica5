#ifndef GENERAL_H
#define GENERAL_H

#include <QCoreApplication>
#include <math.h>
#include <array>
#include <QTime>

//Esta libreria es para colocar tres macros que serán usados dentro de varias de
//las demás clases, además de definir dos funciones que también son usadas en
//varias implementaciones, pero que no poseen una caracteristica particular
//como para incluirlas como métodos de alguna de las clases.

//Éste primer macro es utilizado para activar la visualización de los targets* de
//los fantamas, es decir, ver por medio de una marca el lugar hacia donde se
//quiere mover cada fantasma. El valor de 0 es para desactivarlo y el de 1
//para activarlo.

//*Para más información sobre los targets ver la documentación del movimiento
//de los fantasmas en ghost.cpp, sección MOVIMIENTO Y COLISIONES.

#define GHOSTS_TARGETS 0

//Estas dos macros definen la posición del pixel que se encuentra en la esquina
//superior izquierda del laberinto, el resto de objetos son añadidos a la escena
//según estas dos coordenadas, por lo cual, para modificar la posición donde se
//muestra TODDO el juego (no sólo el laberinto sino también el marcador de score,
//los mensajes y las vidas), basta cambiar estos dos valores.

#define X_MAZE 60
#define Y_MAZE 60

//Éstas dos funciones son de propósito general.

void delay(short mili);
float calculate_dist(short x, short y, short x_tar, short y_tar);

#endif // GENERAL_H
