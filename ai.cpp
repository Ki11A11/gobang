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
                while(++col<=boardWidth && board[col][row]==board[y][x]) ++cnt;
                if(col<=boardWidth && board[col][row]==EMPTY) flag2=true;
                if(flag1 && flag2)
                    res+=board[i][j]*cnt*cnt;
                else if(flag1 || flag2) res+=board[i][j]*cnt*cnt/4;
                if(cnt>=5) res=MAXN*board[i][j];

                col=x;row=y;
                cnt=1;flag1=false;flag2=false;
                while(--row>0 && board[col][row]==board[x][y]) ++cnt;
                if(row>0 && board[col][row]==EMPTY) flag1=true;
                col=x;row=y;
                while(++row<=boardHeight && board[col][row]==board[x][y]) ++cnt;
                if(row<=boardHeight && board[col][row]==EMPTY) flag2=true;
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
                while(++col<=boardWidth && ++row<=boardHeight && board[col][row]==board[x][y]) ++cnt;
                if(col<=boardWidth && row<=boardHeight && board[col][row]==EMPTY) flag2=true;
                if(flag1 && flag2)
                    res+=board[i][j]*cnt*cnt;
                else if(flag1 || flag2) res+=board[i][j]*cnt*cnt/4;
                if(cnt>=5) res=MAXN*board[i][j];

                col=x;row=y;
                cnt=1;flag1=false;flag2=false;
                while(++col<=boardWidth && --row>0 && board[col][row]==board[x][y]) ++cnt;
                if(col<=boardWidth && row>0 && board[col][row]==EMPTY) flag1=true;
                col=x;row=y;
                while(--col>0 && ++row<=boardHeight && board[col][row]==board[x][y]) ++cnt;
                if(col>0 && row<=boardHeight && board[col][row]==EMPTY) flag2=true;
                if(flag1 && flag2)
                    res+=board[i][j]*cnt*cnt;
                else if(flag1 || flag2) res+=board[i][j]*cnt*cnt/4;
                if(cnt>=5) res=MAXN*board[i][j];
            }
        }
    }
    return res;
}
void AI::MakeDecision(){
//    qDebug()<<AI::getMark()
//    emit got_idea(step);
}


//pure communication slots
void AI::Enter(int cur, QVector<QVector<int> >board){
    myTurn=cur;
    qDebug()<<AI::getMark(board,10,10);
    MakeDecision();
}
