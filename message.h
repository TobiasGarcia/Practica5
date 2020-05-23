#ifndef MESSAGE_H
#define MESSAGE_H

#include <QGraphicsTextItem>
#include <QFont>
#include <QTimer>
#include "player.h"

class Message: public QGraphicsTextItem {
private:
    QTimer *timer;
    bool blink_bool;

public:
    Message();
    ~Message() {delete timer;};
    void press_key_msg();
    void ready_msg();
    void go_msg();
    void win_msg();

public slots:
    void blink();

};

#endif // MESSAGE_H
