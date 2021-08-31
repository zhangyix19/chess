#include "chesswidget.h"
//Paint
void ChessWidget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    //绘制背景
    this->background.load(":/back/res/eng/background.png");
    this->unknownman.load(":/back/res/eng/p0.png");
    p.drawPixmap(0,0,651,924,this->background);
    //绘制棋子
    if(this->GameStarted==1)
    for(int i=0;i<5;i++){
        for(int j=0;j<12;j++){
            if(this->position[i][j].status==0)
                ;
            else if(this->position[i][j].status==1)
                p.drawPixmap(this->position[i][j].x,this->position[i][j].y,87,43,this->unknownman);
            else if(this->position[i][j].status==2)
                p.drawPixmap(this->position[i][j].x,this->position[i][j].y,87,43,this->position[i][j].man.icon);
            if(this->Gamer==this->isClient)
            {
                if(this->pfocus==&(this->position[i][j])){
                    QPen pen(Qt::green,5);
                    p.setPen(pen);
                    p.drawRect(this->position[i][j].x,this->position[i][j].y,87,43);
                }
                if(this->position[i][j].target){
                    QPen pen(Qt::green,10);
                    p.setPen(pen);
                    p.drawPoint(this->position[i][j].x+44,this->position[i][j].y+22);
                }
            }
        }
    }

}
//Text
QString ChessWidget::strColor(int gamer)
{
    if(this->ColorAssigned){
        if(this->GamersColor[gamer]==0)return "蓝色";
        else return "红色";
    }else return "未知色";
}
QString ChessWidget::strColor(Man man)
{
        if(man.color==0)return "蓝色";
        else return "红色";

}
void ChessWidget::UpdateStatus(QString str)
{
    if(!str.isEmpty()) emit signaldisplaystatus(str);
    else emit signaldisplaystatus(QString("第%1手\n现在轮到Gamer%2(%3)行棋").arg(QString::number(this->ActionTimer[2]+1)).arg(QString::number(this->Gamer)).arg(strColor(this->Gamer)));
}
//data
GAME ChessWidget::GetGamedata()
{
    GAME data;
    data.GameStarted=this->GameStarted;
    if(data.GameStarted)
    {
        data.focused=this->pfocus!=nullptr;
        if(data.focused){
            data.focusx=this->pfocus->ix;
            data.focusy=this->pfocus->iy;
        }
        for(int i=0;i<3;i++)
            data.ActionTimer[i]=this->ActionTimer[i];
        for(int i=0;i<2;i++)
            for(int j=0;j<2;j++)
                data.ColorArray[i][j]=this->ColorArray[i][j];
        for(int i=0;i<2;i++)
            data.GamersColor[i]=this->GamersColor[i];
        data.ColorAssigned=this->ColorAssigned;
        data.Gamer=this->Gamer;
        for(int i=0;i<2;i++)
            data.OvertimeCounter[i]=this->OvertimeCounter[i];
        data.RemainingTime=this->RemainingTime;
        for(int i=0;i<5;i++)
            for(int j=0;j<12;j++)
            {
                data.posdata[i][j].color=this->position[i][j].man.color;
                data.posdata[i][j].focused=this->position[i][j].focused;
                data.posdata[i][j].ix=i;
                data.posdata[i][j].iy=j;
                data.posdata[i][j].number=this->position[i][j].man.number;
                data.posdata[i][j].status=this->position[i][j].status;
                data.posdata[i][j].target=this->position[i][j].target;
            }
    }
    return data;
}
void ChessWidget::CoverGamedata(GAME data)
{
    this->GameStarted=data.GameStarted;
    if(this->GameStarted){
        if(data.focused)this->pfocus=&this->position[data.focusx][data.focusy];
        else this->pfocus=nullptr;
        for(int i=0;i<3;i++)
            this->ActionTimer[i]=data.ActionTimer[i];
        for(int i=0;i<2;i++)
            for(int j=0;j<2;j++)
                this->ColorArray[i][j]=data.ColorArray[i][j];
        for(int i=0;i<2;i++)
            this->GamersColor[i]=data.GamersColor[i];
        this->ColorAssigned=data.ColorAssigned;
        this->Gamer=data.Gamer;
        for(int i=0;i<2;i++)
            this->OvertimeCounter[i]=data.OvertimeCounter[i];
        this->RemainingTime=data.RemainingTime;
        for(int i=0;i<5;i++)
            for(int j=0;j<12;j++)
            {
                this->position[i][j].man.color=data.posdata[i][j].color;
                this->position[i][j].focused=data.posdata[i][j].focused;
                this->position[i][j].man.number=data.posdata[i][j].number;
                this->position[i][j].status=data.posdata[i][j].status;
                this->position[i][j].target=data.posdata[i][j].target;
                this->position[i][j].x=20+132*i;
                this->position[i][j].y=18+64*j+(j>5)*136;
                this->position[i][j].ix=i;
                this->position[i][j].iy=j;
                if(position[i][j].man.color==0)this->position[i][j].man.icon.load(":/blue/res/eng/b"+QString::number(position[i][j].man.number/10)+QString::number(position[i][j].man.number%10)+".png");
                else this->position[i][j].man.icon.load(":/red/res/eng/r"+QString::number(position[i][j].man.number/10)+QString::number(position[i][j].man.number%10)+".png");
            }
    }
    update();
}
