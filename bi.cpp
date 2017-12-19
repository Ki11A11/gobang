#include "bi.h"
#include <QString>
#include <QDebug>


BI::BI()
{

}

int BI::Evaluate(Step *step)//x,y,所需检测的玩家棋子类型
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
    qDebug()<<"myTurn:"<<myTurn;
    qDebug()<<"point:"<<"("<<step->getX()<<","<<step->getY()<<")";
    qDebug()<<"id:"<<step->getId();
    qDebug()<<"score:"<<score;
    return score;
}

int BI::Getline(Step *step,Direction direct,int distance)
{
    int x=step->getX();
    int y=step->getY();
    switch ((Direction)direct) {
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

void BI::Enter(int myTurn,QVector<QVector<int> > *board)
{
    return;
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
    printSite();


    QVector<Step*> ThisTurnSteps;
    ThisTurnSteps = FindSteps(myTurn);
    int count = ThisTurnSteps.size();
    int best = -1000000;
    int score = 0;
    QVector<Step*> End;
    for(int i=0;i<count;i++)
    {
        Step* s = ThisTurnSteps.at(i);
        chessSite[s->getX()][s->getY()] = myTurn;
        score = Alpha_Beta(deep-1,*s,1000000,-1000000);


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

    qDebug()<<End[end]->getX()<<","<<End[end]->getY();

}

void BI::printSite()
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

QVector<Step*> BI::FindSteps(int plyer)
{
    QVector<Step*> ConsidedStep;
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            Step* step = new Step(i,j,plyer);
            if(Getline(step,Direction::ALL,2)==2)
            {
                ConsidedStep.prepend(step);
            }
        }
    }
    while (!ConsidedStep.isEmpty()) {
        Step* a=ConsidedStep.takeFirst();
        got_idea(a);
        qDebug()<<"("<<a->getX()<<","<<a->getY()<<","<<a->getId()<<")";
    }

    if(ConsidedStep.isEmpty())
    {
        Step* _step = new Step(7,7,plyer);
        ConsidedStep.prepend(_step);
    }
    return ConsidedStep;
}

int BI::Alpha_Beta(int deep,Step step, int alpha, int beta)
{
    if(deep <=0)
    {
        step.setId(myTurn);
        return Evaluate(&step);
    }
    if(deep%2 == 0)
    {
        QVector<Step*> ThisTurnSteps;
        ThisTurnSteps = FindSteps(myTurn);
        int count = ThisTurnSteps.size();
        int best = -1000000;
        int score = 0;
        for(int i=0;i<count;i++)
        {
            Step* s = ThisTurnSteps.at(i);
            chessSite[s->getX()][s->getY()] = myTurn;
            score = Alpha_Beta(deep-1,*s,alpha,best>beta?best:beta);
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
        ThisTurnSteps = FindSteps(myTurn);
        int count = ThisTurnSteps.size();
        int best = 1000000;
        int score = 0;
        for(int i=0;i<count;i++)
        {
            Step* s = ThisTurnSteps.at(i);
            chessSite[s->getX()][s->getY()] = -myTurn;
            score = Alpha_Beta(deep-1,*s,best<alpha?best:alpha,beta);
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

