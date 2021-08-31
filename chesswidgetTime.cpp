#include "chesswidget.h"
#include"mainwindow.h"
void ChessWidget::ChessTimer()
{
    if(this->GameStarted==1&&this->isClient==0){
        this->RemainingTime--;
        emit signalServerupdate(4);
        if(this->RemainingTime==0){
            this->OvertimeCounter[this->Gamer]++;
            if(this->isClient==this->Gamer)emit signalText(QString("Gamer%1:\n您本回合已超时！超时3次将判负！\n您已累计超时%2次！").arg(this->Gamer).arg(this->OvertimeCounter[this->Gamer]));
            else emit signalText(QString("Gamer%1:\n(对手)本回合已超时！超时3次将判负！\n您已累计超时%2次！").arg(this->Gamer).arg(this->OvertimeCounter[this->Gamer]));
            this->ColorArray[this->Gamer][this->ActionTimer[this->Gamer]%2]=2;;;
            if(this->OvertimeCounter[this->Gamer]==3)FinishGame(this->Gamer+3);
            else NextRound();
        }
        SetRemainTime(this->RemainingTime);
    }

}
void ChessWidget::SetRemainTime(int time)
{
    this->RemainingTime=time;
    if(this->GameStarted)emit signaldisplayRamainTime(time);
    else emit signaldisplayRamainTime(0);
}
void ChessWidget::updateClientTimer(){
    if(this->RemainingTime==0&&this->GameStarted==1){
        this->OvertimeCounter[this->Gamer]++;
        if(this->isClient==this->Gamer)emit signalText(QString("Gamer%1:\n您本回合已超时！超时3次将判负！\n您已累计超时%2次！").arg(this->Gamer).arg(this->OvertimeCounter[this->Gamer]));
        else emit signalText(QString("Gamer%1:\n(对手)本回合已超时！超时3次将判负！\n您已累计超时%2次！").arg(this->Gamer).arg(this->OvertimeCounter[this->Gamer]));
        if(this->OvertimeCounter[this->Gamer]==3)FinishGame(this->Gamer+3);
        else this->RemainingTime=20;
    }
    SetRemainTime(this->RemainingTime);
}
