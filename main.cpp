#include <QApplication>
#include <QPushButton>
#include "game.h"
#include "ai.h"
#include <QDebug>
#include "remote.h"

int main(int argc, char **argv)
{
QApplication app(argc,argv);
Game window;
AI ai;
CommunicationManager manager;
QObject::connect(&window,SIGNAL(request_AI(int,QVector<QVector<int> >*)),&ai,SIGNAL(request_Calculator(int,QVector<QVector<int> >*)));
QObject::connect(&ai,SIGNAL(request_Board(Step*)),&window,SLOT(AIDrop(Step*)));
QObject::connect(&window,SIGNAL(switch_DebugMode(int)),&ai,SIGNAL(switch_DebugMode(int)));
QObject::connect(&window,SIGNAL(start_Game()),&manager,SIGNAL(request_Remote()));
window.show();
return app.exec();
}

