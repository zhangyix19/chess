#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QWidget>
#include<QPainter>
#include<QPaintEvent>
#include<QRandomGenerator>
#include<QMouseEvent>
#include<QPaintDevice>
#include<QPaintEngine>
#include<QString>
#include<QTime>
struct Man{
    QString name;
    QPixmap icon;
    //各种子力属性
    int color;
    int number;
    int amount;
    char path[100];
    int gamer;
};

struct Position{
    Man man;
    int x,y,ix,iy;
    int status; //0:空 1:未知棋子 2:已知棋子
    bool focused;//0:未选中 1:选中
    bool target;//0:不可移动 1:可移动
};
# pragma pack(1)
struct PositionData{
    int ix,iy;
    int status;
    bool focused;
    bool target;
    int color;
    int number;
};

struct GAME{
    int GameStarted;
    PositionData posdata[5][12];
    int focused;
    int focusx,focusy;
    int Gamer;
    int ActionTimer[3];
    int ColorAssigned,GamersColor[2],ColorArray[2][2];
    int OvertimeCounter[2],RemainingTime;
};
struct package{
    int status;
    char data[4096];
};
struct clickdata{
    int x,y,lorr;
};

# pragma pack()
class ChessWidget :public QWidget
{
    Q_OBJECT
public:
    Position position[5][12];
    int GameStarted;
    QPixmap background,unknownman;
    Position* pfocus;
    int Gamer;
    int ActionTimer[3];
    int ColorAssigned,GamersColor[2],ColorArray[2][2];
    int OvertimeCounter[2],RemainingTime;
    int isClient,isServer;
public:
    explicit ChessWidget(QWidget *parent = nullptr);
    void mousePressEvent ( QMouseEvent *  );
    int isGameStarted();
    void StartGame();
    GAME GetGamedata();
    void CoverGamedata(GAME);
signals:
    void signalNextRound(int);
    void signalText(QString);
    void signaldisplaystatus(QString);
    void signalappendLog(QString);
    void signaldisplayRamainTime(int);
    void signalClientClick(int, int x=0, int y=0 , int lorr=0);
    void signalServerupdate(int);

public slots:

    void ChessTimer();
protected:
    void paintEvent(QPaintEvent*);
public:
    void Leftclick(int,int);
    void CancleFocus();
    void NextRound();
    void FocusAt(Position*);
    void FindTarget();
    void ClearTarget();
    void TargetGeneraterRailway(Position*);
    void TargetGeneraterRoad(Position*);
    void TargetFilter();
    int Finished();
    void UpdateStatus(QString str="");
    QString strColor(int);
    QString strColor(Man);
    void SetRemainTime(int);
    void FinishGame(int);
    void updateClientTimer();
};

#endif // CHESSWIDGET_H
