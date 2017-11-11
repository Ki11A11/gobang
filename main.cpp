#include <QApplication>
#include <QPushButton>
#include "game.h"

int main(int argc, char **argv)
{
QApplication app(argc,argv);
Game window;
window.show();
return app.exec();
}
