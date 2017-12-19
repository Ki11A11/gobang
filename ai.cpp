#include "ai.h"
#define MAXN 1000000
#include <QDebug>
AI::AI(){

}

//core
int AI::getMark(const QVector<QVector<int> > &board, int boardWidth, int boardHeight){
    int res=0;
    for(int i=0;i<boardWidth;i++){
        for(int j=0;j<boardHeight;j++){
            if(board[i][j]!=EMPTY){

                //row
                bool flag1=false,flag2=false;
                int x=i,y=j;
                int cnt=1;
                int col=x,row=y;
                while(--col>0 && board[col][row]==board[x][y]) ++cnt;
                if(col>0 && board[col][row]==EMPTY) flag1=true;
                col=x;row=y;
                while(++col<boardWidth && board[col][row]==board[y][x]) ++cnt;
                if(col<boardWidth && board[col][row]==EMPTY) flag2=true;
                if(flag1 && flag2)
                    res+=board[i][j]*cnt*cnt;
                else if(flag1 || flag2) res+=board[i][j]*cnt*cnt/4;
                if(cnt>=5) res=MAXN*board[i][j];

                col=x;row=y;
                cnt=1;flag1=false;flag2=false;
                while(--row>0 && board[col][row]==board[x][y]) ++cnt;
                if(row>0 && board[col][row]==EMPTY) flag1=true;
                col=x;row=y;
                while(++row<boardHeight && board[col][row]==board[x][y]) ++cnt;
                if(row<boardHeight && board[col][row]==EMPTY) flag2=true;
                if(flag1 && flag2)
                    res+=board[i][j]*cnt*cnt;
                else if(flag1 || flag2)
                    res+=board[i][j]*cnt*cnt/4;
                if(cnt>=5) res=MAXN*board[i][j];

                col=x;row=y;
                cnt=1;flag1=false;flag2=false;
                while(--col>0 && --row>0 && board[col][row]==board[x][y]) ++cnt;
                if(col>0 && row>0 && board[col][row]==EMPTY) flag1=true;
                col=x;row=y;
                while(++col<boardWidth && ++row<boardHeight && board[col][row]==board[x][y]) ++cnt;
                if(col<boardWidth && row<boardHeight && board[col][row]==EMPTY) flag2=true;
                if(flag1 && flag2)
                    res+=board[i][j]*cnt*cnt;
                else if(flag1 || flag2) res+=board[i][j]*cnt*cnt/4;
                if(cnt>=5) res=MAXN*board[i][j];

                col=x;row=y;
                cnt=1;flag1=false;flag2=false;
                while(++col<boardWidth && --row>0 && board[col][row]==board[x][y]) ++cnt;
                if(col<boardWidth && row>0 && board[col][row]==EMPTY) flag1=true;
                col=x;row=y;
                while(--col>0 && ++row<boardHeight && board[col][row]==board[x][y]) ++cnt;
                if(col>0 && row<boardHeight && board[col][row]==EMPTY) flag2=true;
                if(flag1 && flag2)
                    res+=board[i][j]*cnt*cnt;
                else if(flag1 || flag2) res+=board[i][j]*cnt*cnt/4;
                if(cnt>=5) res=MAXN*board[i][j];
            }
        }
    }
    return res;
}
void AI::gen_step(Node* ro){
    int distance = 2;
    Step* step=nullptr;
    Node* node=nullptr;
    ro->children = QVector<Node*>();
    for(int i=0;i<board->length();i++){
        for(int j=0;j<board->length();j++){
            if(board->at(i).at(j)!=EMPTY){

                int x,y;
                for(int k=-distance;k<=distance;k++){

                    x=k+i;
                    if(0<=x&&x<board->length()){

                        for(int l=-distance;l<=distance;l++){
                            y=l+j;
                            if(0<=y&&y<board->length()){
                                if(board->at(x).at(y)!=EMPTY)continue;
                                step = new Step(x,y,MARK_NEW(myTurn));
                                node = new Node(step,0,nullptr);
                                ro->children.append(node);

                            }
                        }
                    }
                }
            }
        }
    }
}
int AI::max(int deep,Node*ro,int alpha,int beta){
    if(deep==0)return getMark(*(this->board),this->board->length(),this->board->length());
    gen_step(ro);
    Node* node= nullptr;
    Step* step=nullptr;
    Node* bestChild = nullptr;
    int val=-MAXN;
    int best=-MAXN;
    for(int i=0;i<ro->children.length();i++){
        node = ro->children[i];
        step = node->step;
        if(step==nullptr)return best;
        emit put_try(step);
        val = min(deep-1,node,alpha,best>beta?best:beta);
        emit untry();

        if(val>best){

            best = val;
            alpha = best;
            bestChild = node;
        }
        if(val>alpha)
            break;
    }
    ro->mark = best;
    ro->bestChild = bestChild;
    return best;
}
int AI::min(int deep,Node*ro,int alpha,int beta){
    if(deep==0)return getMark(*(this->board),this->board->length(),this->board->length());
    gen_step(ro);
    Node* node= nullptr;
    Step* step=nullptr;
    Node* bestChild = nullptr;
    int val=MAXN;
    int best=MAXN;
    for(int i=0;i<ro->children.length();i++){
        node = ro->children[i];
        step = node->step;
        if(step==nullptr)return best;
        emit put_try(step);
        val = max(deep-1,node,best<alpha?best:alpha,beta);
        emit untry();

        if(val<best){
            best = val;
            beta = best;
            bestChild = node;
        }
        if(val<beta)
            break;
    }
    ro->bestChild = bestChild;
    ro->mark = best;
    return best;
}
void AI::maxmin_alphabeta(){
    if(myTurn>0)max(Deep,root,-MAXN,MAXN);
    else min(Deep,root,-MAXN,MAXN);
}

void AI::MakeDecision(){
    maxmin_alphabeta();
    Node* node = root->bestChild;
    Step* step;
    if(node==nullptr)
        step=new Step(5,5,MARK_NEW(myTurn));
    else
        step = node->step;
    emit got_idea(step);
}


//pure communication slots
void AI::Enter(int cur, QVector<QVector<int> >*board){

    myTurn=cur;
    this->board = board;
    root = new Node(nullptr,0,nullptr);
    MakeDecision();
}
