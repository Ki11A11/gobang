#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <QPixmap>
#include <QPoint>
#include <QVector>
#include <QLabel>
#include <QComboBox>
#include <QMessageBox>
#define WHITE 1
#define BLACK -1
#define EMPTY 0

class Step;
class Game : public QWidget
{
    Q_OBJECT
public:


    explicit Game(QWidget *parent = 0);
signals:
    void switch_DebugMode(int mode);
public slots:
    void AIDrop(Step* step);
    void OnBanishFinished();
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
private:
    int cellSize=50;
    int boardMargin=30;

    int windowWidth=900;
    int windowHeight=1000;
    int canvasWidth=cellSize*14+2*boardMargin;
    int canvasHeight=cellSize*14+2*boardMargin;
    int canvasX = (windowWidth-canvasWidth)/2;
    int canvasY = 200;
    int btnGroupLeftX = canvasX;
    int btnGroupY = canvasY + -125;
    int btnGroupY2 = canvasY -75;

    bool EnableAI = false;
    int Winner = EMPTY;
    int CurPlayer=WHITE;
    int AITurn = EMPTY;
    bool IsBanished = false;

    QPixmap* canvas;
    QPushButton *btn_move,
                *btn_re_move,
                *btn_unmove,
                *btn_invite_AI,
                *btn_banish_AI,
                *btn_re_start;
    QLabel *lb_cb_switch_DebugMode,
           *lb_manifest_AI_INFO;
    QComboBox *cb_switch_DebugMode;

    QVector<Step*> unmovedSteps,
                   steps;
    QVector<QVector<int> > board;


    void initView();
    void initData();
    void getStepFromMouse(int mouseX, int mouseY);
    int Judge(Step* step);
    void Draw();
    void UnMove();
    void ReMove();
    void Move(Step* step);
signals:
    void request_AI(int cur, QVector<QVector<int> >*board);
    void banish_AI();
private slots:
    void BTN_re_move_clicked();
    void BTN_unmove_clicked();
    void BTN_invite_AI_clicked();
    void BTN_banish_AI_clicked();
    void BTN_re_start_clicked();
    void CB_switch_DebugMode_changed(int index);
};

class Step{
public:
    Step(int x,int y,int id);
    Step(QPoint _pos,int id);
    int getX()const;
    int getY()const;
    int getId()const;
private:
    int x;
    int y;
    int id;
};

#endif // GAME_H
