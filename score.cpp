#include "score.h"

Score::Score() {

    QFontDatabase::addApplicationFont(":/resources/arcade.ttf");
    setPlainText(QString("Score: 0"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("arcade normal", 12));
    setPos(80, 25);
}

void Score::increase_score(short points) {
    score += points;
    setPlainText(QString("Score: ") + QString::number(score));
}

void Score::restart_score() {
    score = 0;
    setPlainText(QString("Score: 0"));
    setDefaultTextColor(Qt::white);
}

void Score::win_score() {
    setPlainText(QString("Score: ") + QString::number(score));
    setDefaultTextColor(Qt::yellow);
    setPos(207, 336);
}
