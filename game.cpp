#include "game.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QLabel>
using namespace Qt;
#define MARK_UNMOVE(step,mark__unmove,matrix) (step->setId(step->getId()-10)),(mark__unmove++),matrix[step->getX()][step->getY()]=EMPTY
#define MARK_REMOVE(step,mark__unmove,matrix) (step->setId(step->getId()+10)),(mark__unmove--),matrix[step->getX()][step->getY()]=step->getId()
#define REMOVE_METRICS(id) id==WHITE||id==BLACK
#define GOBANG_SIZE (boardSize+QPoint(1,1))
#define NEW_METRICS(step,matrix) (step->getId()>=10?(step->setId(step->getId()-10),matrix[step->getX()][step->getY()]=step->getId(),true):false)

Game::Game(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setGeometry(50,50,750,750);
    label_recorder = new QLabel(this);
    label_recorder->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label_recorder->setText("WHITE:BLACK");
    label_recorder->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    label_recorder->setGeometry(120,0,200,100);
    canvas = new QPixmap(canvas_width,canvas_height);
    matrix.resize(GOBANG_SIZE.x());
    for(int i=0;i<matrix.length();i++){
        matrix[i].resize(GOBANG_SIZE.y());
        matrix[i].fill(EMPTY,matrix[i].length());
    }
    btn_remove = new QPushButton(this);
    btn_remove->setText("ReMove");
    btn_unmove = new QPushButton(this);
    btn_unmove->setText("UnMove");
    btn_remove->setGeometry(0,0,100,50);
    btn_unmove->setGeometry(0,50,100,50);
    connect(btn_remove,SIGNAL(clicked()),this,SLOT(ReMove()));
    connect(btn_unmove,SIGNAL(clicked()),this,SLOT(UnMove()));
    Draw();
}


//heritance
void Game::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(canvasPos.x(),canvasPos.y(),canvas_width,canvas_height,*canvas);
}
void Game::mousePressEvent(QMouseEvent *event){
    QPoint globalCursorPos = QCursor::pos();
    int mouseScreen = qApp->desktop()->screenNumber(globalCursorPos);
    QRect mouseScreenGeometry = qApp->desktop()->screen(mouseScreen)->geometry();
    QPoint localCursorPos = globalCursorPos - mouseScreenGeometry.topLeft();
    QPoint mousePos = event->pos()-canvasPos-boardMargin;
    mousePos.setX(qRound((float)mousePos.x()/cellSize.x()));
    mousePos.setY(qRound((float)mousePos.y()/cellSize.y()));
    if(0 <= mousePos.x() && mousePos.x() <= boardSize.x()
            && 0<= mousePos.y() && mousePos.y() <= boardSize.y())
    {

        if(matrix[mousePos.x()][mousePos.y()]!=EMPTY)
            return;
        Step* step= new Step(mousePos,curPlayer+10);
        Move(step);
    }
}
void Game::mouseReleaseEvent(QMouseEvent *event){
    Q_UNUSED(event);
}


// setters
void Game::setBoardSize(int width, int height){
    boardSize.setX(width);
    boardSize.setY(height);
}
void Game::setBoardSize(int size){
    setBoardSize(size,size);
}
void Game::setBoardSize(QPoint size){
    boardSize = size;
}
void Game::setCellSize(int width, int height){
    cellSize.setX(width);
    cellSize.setY(height);
}
void Game::setCellSize(int size){
    setCellSize(size,size);
}
void Game::setCellSize(QPoint size){
    cellSize = size;
}

//core
void Game::Draw(){
    QPainter painter(canvas);
    painter.eraseRect(canvas->rect());
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::gray);
    painter.drawRect(canvas->rect());
    QPen pen(Qt::black);
    pen.setWidth(2);
    painter.setPen(pen);
    for(int i=0;i<boardSize.y()+1;i++){
        painter.drawLine(boardMargin.x(),boardMargin.y()+i*cellSize.y(),boardMargin.x()+boardSize.x()*cellSize.x(),boardMargin.y()+i*cellSize.y());
    }
    for(int j=0;j<boardSize.x()+1;j++){
        painter.drawLine(boardMargin.x()+j*cellSize.x(),boardMargin.y(),boardMargin.x()+j*cellSize.x(),boardMargin.y()+boardSize.y()*cellSize.y());
    }
    painter.setPen(Qt::NoPen);
    for(QVector<Step*>::const_iterator it = stepList.begin();it!=stepList.end();it++){
        Step* s = *it;
        int color = s->getId();
        if(color == BLACK){
            painter.setBrush(Qt::black);
        }else if(color ==WHITE){
            painter.setBrush(Qt::white);
        }else {continue;}
        painter.drawEllipse(QPoint(s->getX()*cellSize.x()+boardMargin.x(),s->getY()*cellSize.y()+boardMargin.y()),cellSize.x()/2,cellSize.x()/2);
    }
    repaint();
}
void Game::Move(Step* step){
    if(NEW_METRICS(step,matrix)){
        if(markUnMove==0)
        {
            stepList.append(step);
        }
        else {
            stepList[stepList.length()-markUnMove] = step;
        }
    }

    curPlayer = curPlayer==BLACK?WHITE:BLACK;
    Judge(step);
    Draw();
}
void Game::Move(int x,int y, int id){
    Step* step = new Step(x,y,id);
    Move(step);
}
void Game::UnMove(){
    Step* step;
    int nextRemove;

    if(stepList.length()==0)return;

    nextRemove = stepList.length()-markUnMove-1;
    if(nextRemove<0)return;

    step = stepList[nextRemove];
    MARK_UNMOVE(step,markUnMove,matrix);
    Move(step);
}
void Game::ReMove(){
    Step* step;
    if(markUnMove==0)return;

    step=stepList[stepList.length()-markUnMove];
    MARK_REMOVE(step,markUnMove,matrix);
    Move(step);
}
void Game::Judge(Step* step){
    int co = step->getId();
    int x = step->getX();
    int y = step->getY();
    int c,r,m,v;
    int row,col,mai,vice;
    c=r=m=v=0;
    row=col=mai=vice=0;
    for(int i=-4;i<5;i++){
        c = x+i;
        if(0<=c&&c<GOBANG_SIZE.x()){
            if(row==5)continue;
            if(matrix[c][y]==co){
                row++;
            }else{

                row=0;
            }
        }
    }
    for(int i=-4;i<5;i++){
        r = y+i;
        if(0<=r&&r<GOBANG_SIZE.y()){
            if(col==5)break;
            if(matrix[x][r]==co){
                col++;
            }else{
                col=0;
            }
        }
    }
    for(int i=-4;i<5;i++){
        m = i;
        if(0<=(x+m)&&(x+m)<GOBANG_SIZE.y()&&0<=(y+m)&&(y+m)<GOBANG_SIZE.y() ){
            if(mai==5)break;
            if(matrix[x+m][y+m]==co){
                mai++;
            }else{
                mai=0;
            }
        }
    }

    for(int i=-4;i<5;i++){
        v = i;
        if(0<=(x+v)&&(x+v)<GOBANG_SIZE.x()&&0<=(y-v)&&(y-v)<GOBANG_SIZE.x()){
            if(vice==5)break;
            if(matrix[x+v][y-v]==co){
                vice++;
            }else{
                vice=0;
            }
        }
    }

    if(col==5||row==5||mai==5||vice==5){
        QString winner = co==WHITE?"White":"Black";
        label_recorder->setText(winner);
    }

}


//Step
Step::Step(int x,int y,int id):x(x),y(y),id(id){}
Step::Step(QPoint _pos,int _id){
    x = _pos.x();
    y = _pos.y();
    id = _id;
}
int Step::getX(){return x;}
int Step::getY(){return y;}
int Step::getId(){return id;}
void Step::setId(int id){this->id=id;}
