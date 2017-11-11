#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QPixmap>
#include <QPoint>
#include <QVector>
#include <QLabel>

#define WHITE 1
#define BLACK 0
#define EMPTY -1

class Step;
class Game : public QWidget
{
    Q_OBJECT
public:
    QPoint boardSize=QPoint(10,10);
    QPoint cellSize=QPoint(50,50);
    QPoint boardMargin=QPoint(30,30);
    QVector<Step*> stepList;
    qint32 curPlayer=WHITE; //WHITE go first
    QVector<QVector<int> > matrix;
    explicit Game(QWidget *parent = 0);
    void setBoardSize(int width,int height);
    void setBoardSize(int size);
    void setBoardSize(QPoint size);
    void setCellSize(int width,int height);
    void setCellSize(int size);
    void setCellSize(QPoint size);

    void Move(Step* step);
    void Move(int x,int y, int id);

    void Judge(Step* step);

signals:
public slots:
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
private:
    QPoint canvasPos=QPoint(100,100);
    qint32 window_width=600;
    qint32 window_height=600;
    qint32 canvas_width=cellSize.x()*boardSize.x()+2*boardMargin.x();
    qint32 canvas_height=cellSize.y()*boardSize.y()+2*boardMargin.y();
    QPixmap* canvas;
    QPushButton* btn_move;
    QPushButton* btn_remove;
    QPushButton* btn_unmove;
    QLabel* label_recorder;

    int markUnMove = 0;

private slots:
    void Draw();
    void UnMove();
    void ReMove();
};

class Step{
public:
    Step(int x,int y,int id);
    Step(QPoint _pos,int id);
    int getX();
    int getY();
    int getId();
    void setId(int);
private:
    int x;
    int y;
    int id;
};

#endif // GAME_H
