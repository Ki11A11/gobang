#ifndef BI_H
#define BI_H
#include <QVector>
#include <QPoint>
#include "game.h"
#include <QObject>


enum Direction{Left,Lefttop,Top,Righttop,Right,Rightbottom,Bottom,Leftbottom,ALL};

class BI:public QObject
{
    Q_OBJECT
public:
    BI();
    void printSite();
signals:
    void got_idea(Step* step);
public slots:
    void Enter(int myTurn,QVector<QVector<int> > *board);


private:
    int chessSite[15][15];
    int myTurn;
    int deep = 4;

    int Evaluate(Step*);
    int Getline(Step*,Direction,int);
    int Alpha_Beta(int deep,Step s,int alpha,int beta);
    QVector<Step*> FindSteps(int plyer);
};



#endif // BI_H
