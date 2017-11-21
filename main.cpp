#include <QApplication>
#include <QPushButton>
#include "game.h"
#include "ai.h"
#include <QDebug>

int main(int argc, char **argv)
{
QApplication app(argc,argv);
Game window;
AI ai;
QObject::connect(&window,SIGNAL(notify_ai(int,QVector<QVector<int> >*)),&ai,SLOT(Enter(int,QVector<QVector<int> >*)));
QObject::connect(&ai,SIGNAL(got_idea(Step*)),&window,SLOT(Move(Step*)));
QObject::connect(&ai,SIGNAL(put_try(Step*)),&window,SLOT(Try(Step*)));
QObject::connect(&ai,SIGNAL(untry()),&window,SLOT(UnTry()));
window.show();
return app.exec();
}
