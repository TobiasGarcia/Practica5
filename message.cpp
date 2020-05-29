#include "message.h"

Message::Message() {

    timer = new QTimer;

    setDefaultTextColor(Qt::yellow);
    setFont(QFont("arcade normal", 12));

    //El slot blink() es solo para hacer que titile el mensaje.

    connect(timer, &QTimer::timeout, this, &Message::blink);
    press_key_msg();
}

void Message::press_key_msg() {
    setPlainText(QString("Press any key to start"));
    setPos(X_MAZE + 60, Y_MAZE + 540);
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
    setPos(X_MAZE + 188, Y_MAZE + 277);
}

void Message::go_msg() {
    setPlainText(QString("Go!"));
    setPos(X_MAZE + 214, Y_MAZE + 277);
}

void Message::win_msg() {
    setPlainText(QString("You win!"));
    setPos(X_MAZE + 173, Y_MAZE + 180);
}

void Message::game_over_msg() {
    setPlainText(QString("Game Over"));
    setPos(X_MAZE + 162, Y_MAZE + 180);
}
