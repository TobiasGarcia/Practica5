#include "score.h"

Score::Score() {

    QFontDatabase::addApplicationFont(":/resources/arcade.ttf");
    setPlainText(QString("Score: ") + QString::number(score));
    setDefaultTextColor(Qt::white);
    setFont(QFont("arcade normal", 12));
    setPos(62, 20);
}

void Score::increase_score(short points) {
    score += points;
    setPlainText(QString("Score: ") + QString::number(score));
}
