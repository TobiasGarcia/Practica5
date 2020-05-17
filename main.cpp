#include <QApplication>
#include "pacman.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Pacman *pacman = new Pacman;
    pacman->show();

    return a.exec();
}
