#ifndef AI_H
#define AI_H
#include <QVector>
#include <QPoint>
#include "game.h"
#include <QObject>


enum Direction{Left,Lefttop,Top,Righttop,Right,Rightbottom,Bottom,Leftbottom,ALL};

class AI:public QObject
{
    Q_OBJECT
public:
    AI();
    void printSite();
signals:
    void got_idea(Step* step);
public slots:
    void Enter(int myTurn,QVector<QVector<int> > *board);


private:
    int chessSite[15][15];
    int myTurn;
    int deep = 6;
    int DebugMode = 0;

    int Evaluate(Step*);
    void quickSort(QVector<int>::iterator,QVector<Step*>::iterator,int,int);
    int Getline(Step*,Direction,int);
    int Alpha_Beta(int deep,int alpha,int beta);
    void FindSteps(int player,QVector<Step*>* ConsideredStep);
};



#endif // AI_H
