#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsRectItem>
#include <QTimer>
#include <array>

class Player: public QObject, public QGraphicsRectItem {

    Q_OBJECT

private:
    bool tp = false;
    short width = 25, height = 25, x_maze, y_maze;
    //move_dir [UP, LEFT, DOWN, RIGHT]
    std::array<bool, 4> move_dir;
    QTimer *timer = new QTimer;
public:
    Player(short _x_maze, short _y_maze);
    ~Player() {delete timer;};
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void stop(short x_wall, short y_wall, short width_wall, short height_wall);
signals:
    void earn_point(short points);
public slots:
    void move();
};

#endif // PLAYER_H
