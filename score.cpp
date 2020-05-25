#include "score.h"

Score::Score() {

    score = 0;
    QFontDatabase::addApplicationFont(":/resources/arcade.ttf");
    setPlainText(QString("Score: 0"));
    setDefaultTextColor(Qt::white);
    setFont(QFont("arcade normal", 12));
    setPos(X_MAZE + 20, Y_MAZE - 35);
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

void Score::final_score() {

    short gap = 0;

    if (score == 0) gap = 26;
    else if (score < 100) gap = 17;
    else if (score < 1000) gap = 8;

    setPlainText(QString("Score: ") + QString::number(score));
    setDefaultTextColor(Qt::yellow);
    setPos(X_MAZE + 147 + gap, Y_MAZE + 276);
}
