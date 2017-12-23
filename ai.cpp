#include "ai.h"
#include <QString>
#include <QDebug>

AI::AI()
{

}

int AI::Evaluate(Step *step)//x,y,所需检测的玩家棋子类型
{
    int score = 0;
    int Id = step->getId();
    int countOfTwo = 0;
    for(int direct=0;direct<8;direct++)
    {
        Direction mydir = (Direction)direct;
        if(Getline(step,mydir,1)==Id && Getline(step,mydir,2)==Id
                && Getline(step,mydir,3)==Id && Getline(step,mydir,4)==Id
                && Getline(step,mydir,5)==0)//活四01111*
        {
            score+=300000;
            if(Id!=myTurn)
                score-=500;
            continue;
        }

        if(Getline(step,mydir,1)==Id && Getline(step,mydir,2)==Id
                && Getline(step,mydir,3)==Id && Getline(step,mydir,4)==Id
                && (Getline(step,mydir,5)==-Id || Getline(step,mydir,5)==-2))//死四A21111*
        {
            score+=250000;
            if(Id!=myTurn)
                score-=500;
            continue;
        }

        if(Getline(step,mydir,1)==Id && Getline(step,mydir,2)==Id
                && Getline(step,mydir,3)==Id && Getline(step,mydir,-1)==Id
                )//死四B111*1
        {
            score+=240000;
            if(Id!=myTurn)
                score-=500;
            continue;
        }

        if(Getline(step,mydir,1)==Id && Getline(step,mydir,2)==Id
                && Getline(step,mydir,-1)==Id && Getline(step,mydir,-2)==Id
                )//死四C11*11
        {
            score+=230000;
            if(Id!=myTurn)
                score-=500;
            continue;
        }

        if(Getline(step,mydir,1)==Id && Getline(step,mydir,2)==Id
                && Getline(step,mydir,3)==Id)//活三A111*
        {
            if(Getline(step,mydir,-1)==0)
            {
                score+=750;
                if(Getline(step,mydir,4)==0)
                {
                    score+=3150;
                    if(Id!=myTurn)
                        score-=500;
                }
            }
            if(Getline(step,mydir,-1)==-Id||Getline(step,mydir,-1)==-2&&Getline(step,mydir,4)==0)
            {
                score+=500;
            }

            continue;
        }

        if(Getline(step,mydir,1)==0 && Getline(step,mydir,2)==Id &&
                Getline(step,mydir,3)==Id && Getline(step,mydir,4)==Id)//活三B1110*
        {
            score+=350;
            continue;
        }

        if(Getline(step,mydir,1)==Id && Getline(step,mydir,2)==Id &&
                Getline(step,mydir,-1)==Id)//死三11*1
        {
            score+=600;
            if(Getline(step,mydir,-2)==0 && Getline(step,mydir,3)==0)
            {
                score+=3150;
                continue;
            }

            if(Getline(step,mydir,-2)==-Id || Getline(step,mydir,-2)==-2 &&
                    Getline(step,mydir,3)==-Id || Getline(step,mydir,3)==-2)
            {
                continue;
            }
            else
            {
                score+=700;
                continue;
            }

        }

        if(Getline(step,mydir,1)==Id && Getline(step,mydir,2)==Id &&
                Getline(step,mydir,3)!=-Id && Getline(step,mydir,-1)!=-Id)//活二个数
        {
            countOfTwo++;
        }
        if(countOfTwo>=2)
        {
            score+=3000;
            if(Id!=myTurn)
                score-=100;

            continue;
        }



        int numOfPlayer = 0;//散棋
        for(int k=-4;k<=0;k++)
        {
            int temp = 0;
            for(int l=0;l<=4;l++)
            {
                if(Getline(step,mydir,k+l)==Id)
                {
                    temp++;
                }
                else if(Getline(step,mydir,k+l)==-Id || Getline(step,mydir,k+l)==-2)
                {
                    temp=0;
                    break;
                }
            }
            numOfPlayer+=temp;
        }

        score+=numOfPlayer*15;
    }
    return score;
}

int AI::Getline(Step *step,Direction direct,int distance)
{
    int x=step->getX();
    int y=step->getY();
    switch (direct) {
    case Direction::Left:
        x=x-distance;
        break;
    case Direction::Lefttop:
        x=x-distance;
        y=y-distance;
        break;
    case Direction::Top:
        y=y-distance;
        break;
    case Direction::Righttop:
        x=x+distance;
        y=y-distance;
        break;
    case Direction::Right:
        x=x+distance;
        break;
    case Direction::Rightbottom:
        x=x+distance;
        y=y+distance;
        break;
    case Direction::Bottom:
        y=y+distance;
        break;
    case Direction::Leftbottom:
        x=x-distance;
        y=y+distance;
        break;
    case Direction::ALL:
    {
        if(chessSite[x][y]!=0)
            return 0;
        for(int i=-2;i<=2;i++)
        {
            for(int j=-2;j<=2;j++)
            {
                int nx=x+i;
                int ny=y+j;

                if((nx<0||nx>14||ny<0||ny>14)||(i==0&&j==0))
                    continue;
                else if(chessSite[nx][ny]!=0)
                {
                    return 2;
                }
            }
        }
        return 0;
    }
    default:
        qDebug()<<"invalid direction!"<<direct;
        break;
    }

    if(x<0 || y<0 || x>14 || y>14)
        return -2;

    return chessSite[x][y];
}

void AI::Enter(int myTurn,QVector<QVector<int> > *board)
{

    int i=0;
    this->myTurn = myTurn;
    QVector<QVector<int> >::iterator it = board->begin();
    while(it!=board->end())
    {

        QVector<int> column;
        int j=0;
        column = *it;
        while(!(column.isEmpty()))
        {
            chessSite[j][i]=column.takeFirst();
            j++;
        }
        it++;
        i++;
    }


    if(DebugMode){
        int sumScore = 0;
        for(int i=0;i<15;i++){
            for(int j=0;j<15;j++){
                if(chessSite[i][j]!=0){
                    int symbol = chessSite[i][j]==myTurn?1:-1;
                    Step* s = new Step(i,j,chessSite[i][j]);
                    sumScore += symbol*Evaluate(s);
                }
            }
        }
        qDebug()<<"sumScore:"<<sumScore;
    }

    QVector<Step*> ThisTurnSteps;
    FindSteps(myTurn,&ThisTurnSteps);
    if(DebugMode){

    }else{
        int count = ThisTurnSteps.size();
        int best = -1000000;
        int score = 0;
        QVector<Step*> End;
        for(int i=0;i<count;i++)
        {
            Step* s = ThisTurnSteps.at(i);
            chessSite[s->getX()][s->getY()] = myTurn;
            score = Alpha_Beta(deep-1,1000000,-1000000);
            if(score == best)
            {
                End.push_back(ThisTurnSteps[i]);
            }

            if(score > best)
            {
                best = score;
                End.clear();
                End.push_back(ThisTurnSteps[i]);
            }

            chessSite[s->getX()][s->getY()] = 0;
        }

        count = End.size();
        int end = rand()%count;
        Step* consideredStep = new Step(End[end]->getY(),End[end]->getX(),myTurn+10);
        qDebug()<<consideredStep->getX()<<consideredStep->getY();

        got_idea(consideredStep);
    }

}

void AI::printSite()
{
    for(int i=0;i<15;i++)
    {
        QString Line;
        for(int j=0;j<15;j++)
        {
            Line+=QString::number(chessSite[i][j]);
            Line+=" ";
        }
        qDebug()<<Line;
    }
    qDebug()<<'\n';
}

template<typename T>
void _swap(T *a,T *b){
    T tmp = *a;
    *a = *b;
    *b = tmp;
}


void AI::quickSort(QVector<int>::iterator itScore,QVector<Step*>::iterator itStep,int begin, int end){
    if(end<=begin)return;
    int pivot = *(itScore+begin);
    int i=begin-1,j=end;
    int overwhelming = true;
    while(j>i && i<end && j>begin){
        while(i++<end && j>i&& *(itScore+i)>=pivot);
        while(j-->begin && j>i&&*(itScore+j)<pivot);
        if(i<end && j>begin){
            if(j>i)
            // exchange i and j;
            {
                _swap<int>(itScore+j,itScore+i);
                _swap<Step*>(itStep+j,itStep+i);
            }
            else if(i>=j)
            //i>j: i reached j causing i satisfy cond lt pivot, this way i-1 satisfy gt pivot
            //i==j: j reached i causing i satisfy cond lt pivot either, this way i-1 satisfy gt pivot
            // if i and j never met, mark overwhelming state as true, suggesting i reached the end
            {
                 overwhelming = false;
            }

        }
    }
    _swap<int>(itScore+i-1,itScore+begin);
    _swap<Step*>(itStep+i-1,itStep+begin);
    if(overwhelming){
        quickSort(itScore,itStep,begin,i-1);
    }
    else{
        quickSort(itScore,itStep,begin,i);
        quickSort(itScore,itStep,i,end);
    }
}

void AI::FindSteps(int player, QVector<Step*>* ConsideredStep)
{
    QVector<int> scores;
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            if(chessSite[i][j]!=0){
                continue;
            }
            Step* step = new Step(i,j,player);
            if(Getline(step,Direction::ALL,2)==2)
            {
                Step* otherStep = new Step(i,j,-player);
                ConsideredStep->prepend(step);
                scores.prepend(Evaluate(step)+Evaluate(otherStep));

            }
        }
    }

    if(ConsideredStep->isEmpty())
    {
        Step* _step = new Step(7,7,player);
        ConsideredStep->prepend(_step);
    }
    else {

        QVector<int>::iterator itScore = scores.begin();
        QVector<Step*>::iterator itStep  = ConsideredStep->begin();
        if(DebugMode){
            qDebug()<<"ordinal scores";
            for(int i=0;i<ConsideredStep->length();i++){
                qDebug()<<scores[i];
            }
        }
        quickSort(itScore,itStep,0,ConsideredStep->length());
        if(DebugMode){
            qDebug()<<"sorted scores";
            for(int i=0;i<ConsideredStep->length();i++){
                qDebug()<<scores[i];
            }
            qDebug()<<"---------------------------------------------------";
        }
        if(ConsideredStep->length()>10){
            ConsideredStep->resize(10);
            ConsideredStep->squeeze();
        }
        if(DebugMode){
            qDebug()<<ConsideredStep->at(0)->getY()<<","<<ConsideredStep->at(0)->getX()<<"is the best"<<": "<<scores[0];
        }

    }

}

int AI::Alpha_Beta(int deep,int alpha, int beta)
{
    if(deep <=0)
    {
        int sumScore = 0;
        for(int i=0;i<15;i++){
            for(int j=0;j<15;j++){
                if(chessSite[i][j]!=0){
                    int symbol = chessSite[i][j]==myTurn?1:-1;
                    Step* s = new Step(i,j,chessSite[i][j]);
                    sumScore += symbol*Evaluate(s);
                }

            }
        }
        return sumScore;
    }
    if(deep%2 == 0)
    {
        QVector<Step*> ThisTurnSteps;
        FindSteps(myTurn,&ThisTurnSteps);
        int count = ThisTurnSteps.size();
        int best = -1000000;
        int score = 0;
        for(int i=0;i<count;i++)
        {
            Step* s = ThisTurnSteps.at(i);
            chessSite[s->getX()][s->getY()] = s->getId();
            score = Alpha_Beta(deep-1,alpha,best>beta?best:beta);
            chessSite[s->getX()][s->getY()] = 0;

            if(score > best)
            {
                best = score;
            }

            if(score > alpha)
                break;
        }
        return best;
    }
    else if(deep%2 == 1)
    {
        QVector<Step*> ThisTurnSteps;
        FindSteps(-myTurn,&ThisTurnSteps);
        int count = ThisTurnSteps.size();
        int best = 1000000;
        int score = 0;
        for(int i=0;i<count;i++)
        {
            Step* s = ThisTurnSteps.at(i);
            chessSite[s->getX()][s->getY()] = s->getId();
            score = Alpha_Beta(deep-1,best<alpha?best:alpha,beta);
            chessSite[s->getX()][s->getY()] = 0;

            if(score < best)
            {
                best = score;
            }

            if(score < beta)
                break;
        }
        return best;
    }
    else
    {
        qDebug()<<"error";
        exit(0);
    }
}

