#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include <QFont>
#include <QFontDatabase>

class Score: public QGraphicsTextItem {
private:
    int score = 0;
public:
    Score();
    ~Score() {};
public slots:
    void increase_score(short points);
    void restart_score();
    void win_score();
};

#endif // SCORE_H
