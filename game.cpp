#include "game.h"
#include <QPainter>
#include <QPen>
#include <QDebug>
#include <QApplication>
#include <QMouseEvent>
#include <QLabel>


Game::Game(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    setGeometry(50,50,windowWidth,windowHeight);
    initData();
    initView();
    emit start_Game();
}

void Game::initView(){
    canvas = new QPixmap(canvasWidth,canvasHeight);

    cb_switch_DebugMode = new QComboBox(this);
    cb_switch_DebugMode->setGeometry(canvasWidth+canvasX-200,btnGroupY2,200,50);
    cb_switch_DebugMode->addItem("Evaluate Steps",QVariant(1));
    cb_switch_DebugMode->addItem("Evaluate Board",QVariant(2));
    cb_switch_DebugMode->addItem("Disable Debug",QVariant(0));
    cb_switch_DebugMode->setCurrentIndex(-1);

    lb_cb_switch_DebugMode = new QLabel(this);
    lb_cb_switch_DebugMode->setText("Debug Mode");
    lb_cb_switch_DebugMode->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    lb_cb_switch_DebugMode->setGeometry(canvasWidth+canvasX-150,btnGroupY,100,50);

    lb_manifest_AI_INFO = new QLabel(this);
    lb_manifest_AI_INFO->setText("AI Joined");
    lb_manifest_AI_INFO->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    lb_manifest_AI_INFO->setGeometry(btnGroupLeftX,btnGroupY-50,200,50);
    lb_manifest_AI_INFO->hide();

    btn_re_start = new QPushButton(this);
    btn_re_start->setText("ReStart");
    btn_re_start->setGeometry(windowWidth/2-100,btnGroupY,150,50);
    btn_re_move = new QPushButton(this);
    btn_re_move->setText("Move Again");
    btn_re_move->setGeometry(btnGroupLeftX,btnGroupY,150,50);
    btn_unmove = new QPushButton(this);
    btn_unmove->setText("UnMove");
    btn_unmove->setGeometry(btnGroupLeftX,btnGroupY2,150,50);
    btn_invite_AI = new QPushButton(this);
    btn_invite_AI->setText("Invite AI");
    btn_invite_AI->setGeometry(btnGroupLeftX+150,btnGroupY,150,50);
    btn_banish_AI = new QPushButton(this);
    btn_banish_AI->setText("Banish AI");
    btn_banish_AI->setGeometry(btnGroupLeftX+150,btnGroupY2,150,50);

    connect(btn_re_move,SIGNAL(clicked()),this,SLOT(BTN_re_move_clicked()));
    connect(btn_unmove,SIGNAL(clicked()),this,SLOT(BTN_unmove_clicked()));
    connect(btn_invite_AI,SIGNAL(clicked()),this,SLOT(BTN_invite_AI_clicked()));
    connect(btn_banish_AI,SIGNAL(clicked()),this,SLOT(BTN_banish_AI_clicked()));
    connect(btn_re_start,SIGNAL(clicked()),this,SLOT(BTN_re_start_clicked()));
    connect(cb_switch_DebugMode,SIGNAL(currentIndexChanged(int)),this,SLOT(CB_switch_DebugMode_changed(int)));
    Draw();
}

void Game::initData(){
    board.resize(15);
    for(int i=0;i<board.length();i++){
        board[i].resize(15);
        board[i].fill(EMPTY,board[i].length());
    }
}

void Game::AIDrop(Step *step){
    if(!EnableAI)return;
    Move(step);
    lb_manifest_AI_INFO->setText("AI Droped.");
}
//heritance
void Game::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter p(this);
    p.drawPixmap(canvasX,canvasY,canvasWidth,canvasHeight,*canvas);
}
void Game::mousePressEvent(QMouseEvent *event){
    int eventX = event->pos().x();
    int eventY = event->pos().y();
    getStepFromMouse(eventX,eventY);
}



//core
void Game::getStepFromMouse(int mouseX, int mouseY){
    int x = qRound((float)(mouseX-canvasX-boardMargin)/cellSize);
    int y = qRound((float)(mouseY-canvasY-boardMargin)/cellSize);
    if(0 <= x && x < 15
            && 0<= y && y < 15)
    {
        int turn = CurPlayer==AITurn?EMPTY:CurPlayer;
        Step* step= new Step(x,y,turn);
        Move(step);
    }
}
void Game::Draw(){
    QPainter p(canvas);
    p.eraseRect(canvas->rect());
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::gray);
    p.drawRect(canvas->rect());
    QPen pen(Qt::black);
    pen.setWidth(2);
    p.setPen(pen);
    for(int i=0;i<15;i++){
        p.drawLine(boardMargin,boardMargin+i*cellSize,boardMargin+14*cellSize,boardMargin+i*cellSize);
        p.drawLine(boardMargin+i*cellSize,boardMargin,boardMargin+i*cellSize,boardMargin+14*cellSize);
    }
    p.setPen(Qt::NoPen);
    for(QVector<Step*>::const_iterator it = steps.begin();it!=steps.end();it++){
        Step* s = *it;
        int color = s->getId();
        if(color == BLACK){
            p.setBrush(Qt::black);
        }else if(color ==WHITE){
            p.setBrush(Qt::white);
        }else {continue;}
        p.drawEllipse(QPoint(s->getX()*cellSize+boardMargin,s->getY()*cellSize+boardMargin),cellSize/2,cellSize/2);
    }
    repaint();
}

void Game::BTN_re_move_clicked(){
    ReMove();
}
void Game::BTN_unmove_clicked(){
    UnMove();
}
void Game::BTN_invite_AI_clicked(){
    if(EnableAI)return;
    EnableAI = true;
    AITurn = CurPlayer;
    lb_manifest_AI_INFO->setText("AI joined.");
    lb_manifest_AI_INFO->show();
    emit request_AI(CurPlayer,&board);
}
void Game::BTN_banish_AI_clicked(){
    if(!EnableAI)return;
    EnableAI = false;
    AITurn = EMPTY;
    lb_manifest_AI_INFO->hide();
}
void Game::BTN_re_start_clicked(){
    Winner = EMPTY;
    BTN_banish_AI_clicked();
    steps.clear();
    steps.squeeze();
    unmovedSteps.clear();
    unmovedSteps.squeeze();
    board.clear();
    board.squeeze();
    initData();
    Draw();
    emit start_Game();
}
void Game::CB_switch_DebugMode_changed(int index){
    int mode = cb_switch_DebugMode->itemData(index).toInt();
    emit switch_DebugMode(mode);
}

void Game::Move(Step* step){
    int x = step->getX();
    int y = step->getY();
    int id = step->getId();

    if(id!=CurPlayer || board[x][y]!=EMPTY || Winner!=EMPTY){
        return;
    }
    if(!unmovedSteps.isEmpty()){
        unmovedSteps.clear();
        unmovedSteps.squeeze();
    }
    steps.append(step);
    board[x][y] = id;
    CurPlayer = id==WHITE?BLACK:WHITE;
    Draw();
    Judge(step);
    if(EnableAI && CurPlayer==AITurn){
        emit request_AI(CurPlayer,&board);
        lb_manifest_AI_INFO->setText("AI is pondering.");
    }
}
void Game::UnMove(){
      Step* s = steps.takeLast();
      board[s->getX()][s->getY()] = EMPTY;
      unmovedSteps.append(s);
      Draw();
}
void Game::ReMove(){
    if(!unmovedSteps.isEmpty()){
        Step* s = unmovedSteps.takeLast();
        board[s->getX()][s->getY()] = s->getId();
        steps.append(s);
        Draw();
    }
}
int Game::Judge(Step* step){
    int id = step->getId();
    int x = step->getX();
    int y = step->getY();
    int c,r,m,v;
    int row,col,mai,vice;
    c=r=m=v=0;
    row=col=mai=vice=0;
    for(int i=-4;i<5;i++){
        c = x+i;
        if(0<=c&&c<15){
            if(row==5)continue;
            if(board[c][y]==id){
                row++;
            }else{

                row=0;
            }
        }
    }
    for(int i=-4;i<5;i++){
        r = y+i;
        if(0<=r&&r<15){
            if(col==5)break;
            if(board[x][r]==id){
                col++;
            }else{
                col=0;
            }
        }
    }
    for(int i=-4;i<5;i++){
        m = i;
        if(0<=(x+m)&&(x+m)<15&&0<=(y+m)&&(y+m)<15){
            if(mai==5)break;
            if(board[x+m][y+m]==id){
                mai++;
            }else{
                mai=0;
            }
        }
    }

    for(int i=-4;i<5;i++){
        v = i;
        if(0<=(x+v)&&(x+v)<15&&0<=(y-v)&&(y-v)<15){
            if(vice==5)break;
            if(board[x+v][y-v]==id){
                vice++;
            }else{
                vice=0;
            }
        }
    }

    if(col==5||row==5||mai==5||vice==5){
        Winner = id;
        QString s = Winner==WHITE?"White wins":"Black wins";
        QMessageBox mb;
        mb.setText(s);
        mb.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mb.setButtonText(QMessageBox::Yes,"Play again.");
        mb.setButtonText(QMessageBox::No,"I see it.");
        switch(mb.exec()){
            case QMessageBox::Yes:
                BTN_re_start_clicked();
              break;
        case QMessageBox::No:
            break;
        }
        return CurPlayer;
    }

    return 0;

}
void Game::OnBanishFinished(){
    IsBanished = true;
}

//Step
Step::Step(int x,int y,int id):x(x),y(y),id(id){}
Step::Step(QPoint _pos,int _id){
    x = _pos.x();
    y = _pos.y();
    id = _id;
}
int Step::getX()const{return x;}
int Step::getY()const{return y;}
int Step::getId()const{return id;}
