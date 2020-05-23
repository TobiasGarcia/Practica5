#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include <array>
#include "ghost.h"
#include <QDebug>
#include <QTime>
#include <QCoreApplication>

class Player: public QObject, public QGraphicsPixmapItem {

    Q_OBJECT

private:

    bool is_playing = false;
    QRectF boundingRect() const;

    QPixmap *script;
    short dir = 0;

    short points_left = 150; //150

    bool tp = false, freeze;
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
    short num_script = 0;

    Player(short _x_maze, short _y_maze);
    ~Player() {delete timer; delete[] script;};
    void focusOutEvent(QFocusEvent *event);
    void set_freeze(bool _freeze) {freeze = _freeze;};
    void initialize();
    void win_animation();

signals:
    void earn_point(short points);
    void new_target(short x_pac, short y_pac, short dir_pac);
    void scare_ghosts();
    void touched_ghost();
    void no_points_left();
    void begin();

public slots:
    void move();
    void lose_animation();
};

void delay(short mili);

#endif // PLAYER_H
