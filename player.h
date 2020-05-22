#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <array>
#include "ghost.h"
#include <QDebug>

class Player: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    QRectF boundingRect() const;

    QPixmap *script;
    short num_script = 0, dir = 0;

    bool tp = false;
    short width = 25, height = 25, pixels = 5, last_presesed = 0,
    x_maze, y_maze, gap[4] = {0, -1, 0, 1};
    //move_dir [UP, LEFT, DOWN, RIGHT]
    std::array<bool, 4> pressed_dir, move_dir;
    QTimer *timer = new QTimer;
    QList <QGraphicsItem*> collisions;

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void stop(short x_wall, short y_wall, short width_wall, short height_wall);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    Player(short _x_maze, short _y_maze);
    ~Player() {delete timer; delete[] script;};

signals:
    void earn_point(short points);
    void new_target(short x_pac, short y_pac, short dir_pac);
    void scare_ghosts();

public slots:
    void move();
};

#endif // PLAYER_H
