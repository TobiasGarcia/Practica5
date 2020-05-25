#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include <QFontDatabase>
#include <QFont>
#include "general.h"

class Score: public QGraphicsTextItem {
private:
    int score;

public:
    Score();
    ~Score() {};

public slots:
    void final_score();
    void restart_score();
    void increase_score(short points);

};

#endif // SCORE_H
