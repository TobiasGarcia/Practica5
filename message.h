#ifndef MESSAGE_H
#define MESSAGE_H

#include <QGraphicsTextItem>
#include <QTimer>
#include <QFont>
#include "player.h"
#include "general.h"

//Ésta clase es utilizada para colocar los mensajes que salen en el juego,
//como el de "Press any key to start", "Game Over", etc.

class Message: public QGraphicsTextItem {
private:
    QTimer *timer;
    bool blink_bool;

public:
    Message();
    void go_msg();
    void win_msg();
    void ready_msg();
    void press_key_msg();
    void game_over_msg();
    ~Message() {delete timer;};

public slots:
    void blink();

};

#endif // MESSAGE_H
