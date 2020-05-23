#include "message.h"

Message::Message() {

    timer = new QTimer;

    setDefaultTextColor(Qt::yellow);
    setFont(QFont("arcade normal", 12));

    connect(timer, &QTimer::timeout, this, &Message::blink);
    press_key_msg();
}

void Message::press_key_msg() {
    setPlainText(QString("Press any key to start"));
    setPos(120, 600);
    blink_bool = true;
    timer->start(500);
}
void Message::blink() {
    if (blink_bool) {
        setDefaultTextColor(Qt::black);
        blink_bool = false;
    }
    else {
        setDefaultTextColor(Qt::yellow);
        blink_bool = true;
    }
}

void Message::ready_msg() {
    timer->stop();
    setDefaultTextColor(Qt::yellow);
    setPlainText(QString("Ready?"));
    setPos(248, 337);
}

void Message::go_msg() {
    setPlainText(QString("Go!"));
    setPos(274, 337);
}

void Message::win_msg() {
    setPlainText(QString("You win!"));
    setPos(233, 240);
}
