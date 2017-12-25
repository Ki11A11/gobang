#ifndef AI_H
#define AI_H
#include <QVector>
#include <QPoint>
#include "game.h"
#include <QObject>
#include <QThread>

enum Direction{Left,Lefttop,Top,Righttop,Right,Rightbottom,Bottom,Leftbottom,ALL};

class Calculator;

class AI:public QObject
{
    Q_OBJECT
public:
    AI();
    ~AI();
signals:
    void request_Board(Step* step);
    void request_Calculator(int myTurn,QVector<QVector<int> > *board);
    void switch_DebugMode(int mode);
private:
    QThread calculateThread;

};

class Calculator:public QObject{
    Q_OBJECT
public:
    int deep=6;
    int DebugMode = 0;
    int myTurn;
    int chessSite[15][15];
    void printSite();
signals:
    void return_AI(Step* step);
public slots:
    void Calculate(int myTurn,QVector<QVector<int> > *board);
    void switchDebugMode(int mode);
    int Evaluate(Step*);
    void quickSort(QVector<int>::iterator,QVector<Step*>::iterator,int,int);
    int Getline(Step*,Direction,int);
    int Alpha_Beta(int deep,int alpha,int beta);
    void FindSteps(int player,QVector<Step*>* ConsideredStep);
};

#endif // AI_H
