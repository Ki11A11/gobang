#ifndef AI_H
#define AI_H
#include <QVector>
#include <QPoint>
#include "game.h"
class Node;

class AI:public QObject{
    Q_OBJECT
public:
    explicit AI();
    static int getMark(QVector<QVector<int>> const & board,int boardWidth,int boardHeight);

signals:
    void got_idea(Step* step);
	void put_try(Step* step);
    void untry();
public slots:
    void Enter(int myTurn,QVector<QVector<int> > *board);
private:
    int Deep=4;
    Node* root;
    int myTurn;
    int myMark;
    QVector<QVector<int> >* board;
    void MakeDecision();
    void maxmin_alphabeta();//alpha represent max,beat represent min
    int max(int deep,Node* root,int alpha,int beta);//alpha set to -MAXN,vice the versa beta
    int min(int deep,Node* root,int alpha,int beta);
    void gen_step(Node* ro);
};

class Node{
public:
    Node(Step* step,int mark,Node* node):step(step),mark(mark),bestChild(node){}
    QVector<Node*> children;
    Step* step;
    Node* bestChild;
    int mark;
};

#endif // AI_H
