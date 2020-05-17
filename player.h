#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <array>

class Player: public QObject, public QGraphicsRectItem {

    Q_OBJECT

private:
    //move_dir [UP, LEFT, DOWN, RIGHT]
    std::array<bool, 4> move_dir;
public:
    Player(short x, short y);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void move();
};

#endif // PLAYER_H
