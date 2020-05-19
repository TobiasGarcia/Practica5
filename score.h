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
};

#endif // SCORE_H
