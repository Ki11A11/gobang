#ifndef AI_H
#define AI_H
#include <QVector>
#include <QPoint>
#include "game.h"
class AI:public QObject{
    Q_OBJECT
public:
    explicit AI();
    static int getMark(QVector<QVector<int>> const & board,int boardWidth,int boardHeight);
signals:
    void got_idea(Step* step);
public slots:
    void Enter(int myTurn,QVector<QVector<int> > board);
private:
    int myTurn;
    void MakeDecision();
};

#endif // AI_H
