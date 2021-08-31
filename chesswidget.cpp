#include "chesswidget.h"

ChessWidget::ChessWidget(QWidget *parent):QWidget(parent)
{
    GameStarted=0;
    isClient=0;
    isServer=0;

    pfocus=nullptr;
    for(int i=0;i<3;i++)ActionTimer[i]=0;

    ColorAssigned=0;
    GamersColor[0]=GamersColor[1]=0;
    OvertimeCounter[0]=OvertimeCounter[1]=0;
    for(int i=0;i<2;i++)
        for(int j=0;j<2;j++)
            ColorArray[i][j]=2;
}

