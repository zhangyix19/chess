#include "chesswidget.h"
#include"mainwindow.h"
int ChessWidget::isGameStarted(){return GameStarted;}
void ChessWidget::StartGame()
{
    for(int i=0;i<5;i++){
        for(int j=0;j<12;j++){
            this->position[i][j].status=1;
            this->position[i][j].x=20+132*i;
            this->position[i][j].y=18+64*j+(j>5)*136;
            this->position[i][j].ix=i;
            this->position[i][j].iy=j;
            this->position[i][j].focused=false;
            this->position[i][j].target=false;
        }
    }
    int EmptyPosition[10][2]={{1,2},{3,2},{2,3},{1,4},{3,4},{1,7},{3,7},{2,8},{1,9},{3,9}};
    for(int i=0;i<10;i++)this->position[EmptyPosition[i][0]][EmptyPosition[i][1]].status=0;
    //加载所有棋子
    Man man[24];//blue:0-11 red:12-23
    QString ManNames[12]={"地雷","工兵","排长","连长","营长","团长","旅长","师长","军长","司令","炸弹","军旗"};
    for(int i=0;i<12;i++){
        //amount
        if(i<4) man[i].amount=man[i+12].amount=3;
        else if((i>=4&&i<8)||i==10) man[i].amount=man[i+12].amount=2;
        else man[i].amount=man[i+12].amount=1;
        //icon
        man[i].icon.load(":/blue/res/eng/b"+QString::number(i/10)+QString::number(i%10)+".png");
        man[i+12].icon.load(":/red/res/eng/r"+QString::number(i/10)+QString::number(i%10)+".png");
        //color
        man[i].color=0;
        man[i+12].color=1;
        //number
        man[i].number=man[i+12].number=i;
        //gamer
        man[i].gamer=man[i+12].gamer=2;
        //name
        man[i].name=man[i+12].name=ManNames[i];
    }
    QRandomGenerator r(QTime::currentTime().msec());
    for(int i=0;i<5;i++){
        for(int j=0;j<12;j++){
            if(this->position[i][j].status==1){
                int randnum=r.generate()%24;
                while(man[randnum%24].amount==0)randnum+=1;
                this->position[i][j].man=man[randnum%24];
                man[randnum%24].amount--;
            }
        }
    }
    //初始化变量
    this->GameStarted=1;
    this->pfocus=nullptr;
    for(int i=0;i<3;i++)this->ActionTimer[i]=0;
    this->Gamer=r.generate()%2;
    this->ColorAssigned=0;
    this->GamersColor[0]=this->GamersColor[1]=0;
    this->OvertimeCounter[0]=this->OvertimeCounter[1]=0;
    SetRemainTime(20);
    for(int i=0;i<2;i++)
        for(int j=0;j<2;j++)
            this->ColorArray[i][j]=2;

    emit signalText(tr("游戏开始！Gamer")+QString::number(this->Gamer)+tr("先行！"));
    emit signalappendLog(tr("游戏开始！"));
    update();
    UpdateStatus();
}

void ChessWidget::mousePressEvent ( QMouseEvent * e )//鼠标单击事件响应
{
    int x=e->localPos().x(),y=e->localPos().y();
    if(this->GameStarted==1){
        if(this->isClient==0){
            if(this->Gamer==0)
            {
                if(e->button()==Qt::LeftButton){
                    Leftclick(x,y);
                }else if(e->button()==Qt::RightButton){
                    CancleFocus();
                }
                emit signalServerupdate(4);
            }
        }else{
            if(this->Gamer==1)
            {
                if(e->button()==Qt::LeftButton)emit signalClientClick(5,x,y,0);
                else if(e->button()==Qt::RightButton)emit signalClientClick(5,x,y,1);
            }

        }
    }
}

void ChessWidget::Leftclick(int x,int y)
{

    //判断点击位置
    if(this->GameStarted==0)return;
    bool valid=false;
    int pos_i=0,pos_j=0;
    for(int i=0;i<5;i++){
        for(int j=0;j<12;j++){
            if(this->position[i][j].x<x&&x<this->position[i][j].x+87&&this->position[i][j].y<y&&y<this->position[i][j].y+43){
                valid=true;
                pos_i=i;
                pos_j=j;
            }
        }
    }
    Position* now=&(this->position[pos_i][pos_j]);
    //根据点击位置确定命令逻辑
    //可以信号槽实现
    if(this->ColorAssigned==0){
        //未确定双方颜色
        if(valid&&now->status==1){
            now->status=2;
            emit signalappendLog(QString("第%1手：Gamer%2(%3)\n\t翻开%4:%5（%6，%7）").arg(this->ActionTimer[2]+1).arg(this->Gamer).arg(strColor(this->Gamer)).arg(strColor(now->man)).arg(now->man.name).arg(pos_i).arg(pos_j));
            this->ColorArray[this->Gamer][this->ActionTimer[this->Gamer]%2]=now->man.color;
            if(this->ColorArray[this->Gamer][0]==this->ColorArray[this->Gamer][1]&&this->ColorArray[this->Gamer][0]!=2){
                //分配颜色,gamer[this->Gamer]的颜色为this->ColorArray[this->Gamer][0]
                //若this->Gamer==this->ColorArray[this->Gamer][0]，为0011型，反之为0110型
                this->ColorAssigned=1;
                this->GamersColor[0]=this->Gamer!=this->ColorArray[this->Gamer][0];
                this->GamersColor[1]=this->Gamer==this->ColorArray[this->Gamer][0];
                QString text=QString("Gamer%1为%2！\n").arg(this->Gamer).arg(strColor(this->Gamer))+QString("Gamer%1为%2！").arg(!this->Gamer).arg(strColor(!this->Gamer));
                update();
                emit signalServerupdate(8);
                emit signalappendLog(text);
                emit signalText(text);
            }
            NextRound();
        }
    }else{
        //已确定双方颜色
        if(this->pfocus==nullptr){
            //不聚焦
            if(!valid){
                ;
            }else{
                if(now->status==0){
                    ;
                }else if(now->status==1){
                    //翻棋
                    now->status=2;
                    emit signalappendLog(QString("第%1手：Gamer%2(%3)\n\t翻开%4:%5（%6，%7）").arg(this->ActionTimer[2]+1).arg(this->Gamer).arg(strColor(this->Gamer)).arg(strColor(now->man)).arg(now->man.name).arg(pos_i).arg(pos_j));
                    NextRound();
                }else{
                    if(now->man.color==this->GamersColor[this->Gamer]){
                        //聚焦
                        FocusAt(now);

                    }
                }
            }
        }else{
            //已聚焦
            if(!valid){
                CancleFocus();
            }else{
                //核心逻辑

                if(now->target==true){
                    if(now->status==0){
                        emit signalappendLog(QString("第%1手：Gamer%2(%3)\n\t移动%4%5(%6,%7)至(%8,%9)").arg(this->ActionTimer[2]+1).arg(this->Gamer).arg(strColor(this->Gamer)).arg(strColor(this->Gamer)).arg(this->pfocus->man.name).arg(this->pfocus->ix).arg(this->pfocus->iy).arg(pos_i).arg(pos_j));
                        now->status=2;
                        now->man=this->pfocus->man;
                        this->pfocus->status=0;
                        NextRound();
                    }else if(now->status==2){
                        if(this->pfocus->man.number==10||this->pfocus->man.number==now->man.number||now->man.number==10){
                            emit signalappendLog(QString("第%1手：Gamer%2(%3)\n\t移动%4%5(%6,%7)与%8%9(%10,%11)同归于尽").arg(this->ActionTimer[2]+1).arg(this->Gamer).arg(strColor(this->Gamer)).arg(strColor(this->Gamer)).arg(this->pfocus->man.name).arg(this->pfocus->ix).arg(this->pfocus->iy).arg(strColor(!this->Gamer)).arg(now->man.name).arg(pos_i).arg(pos_j));
                            this->pfocus->status=0;
                            now->status=0;
                            NextRound();
                        }else{
                            emit signalappendLog(QString("第%1手：Gamer%2(%3)\n\t移动%4%5(%6,%7)消灭%8%9(%10,%11)").arg(this->ActionTimer[2]+1).arg(this->Gamer).arg(strColor(this->Gamer)).arg(strColor(this->Gamer)).arg(this->pfocus->man.name).arg(this->pfocus->ix).arg(this->pfocus->iy).arg(strColor(!this->Gamer)).arg(now->man.name).arg(pos_i).arg(pos_j));
                            now->status=2;
                            now->man=this->pfocus->man;
                            this->pfocus->status=0;
                            NextRound();
                        }
                    }
                }else{
                    if(now->man.color==this->GamersColor[this->Gamer]&&now->status==2){
                        CancleFocus();
                        FocusAt(now);
                    }
                    ;
                }
            }
        }
    }

}

void ChessWidget::NextRound()//回合更替
{
    this->ActionTimer[this->Gamer]++;
    this->ActionTimer[2]++;
    this->Gamer=!this->Gamer;
    SetRemainTime(20);
    CancleFocus();
    update();
    UpdateStatus();
    FinishGame(Finished());
    emit signalServerupdate(4);
}

int ChessWidget::Finished()//胜负判定
{

    int result=0;//0:棋局未完成 1:蓝胜 2:红胜
    int blueflag=0,redflag=0,gamercanmove=0;
    for(int i=0;i<5;i++)
        for(int j=0;j<12;j++)
            if(this->position[i][j].status!=0)
            {
                if(this->position[i][j].man.number==11&&this->position[i][j].man.color==0)blueflag=1;
                if(this->position[i][j].man.number==11&&this->position[i][j].man.color==1)redflag=1;
                if(this->position[i][j].status==1)gamercanmove=1;
                else if(this->position[i][j].status==2&&this->position[i][j].man.color==this->GamersColor[this->Gamer]){
                    FocusAt(&this->position[i][j]);
                    for(int p=0;p<5;p++)
                        for(int q=0;q<12;q++)
                            if(this->position[p][q].target==1)
                                gamercanmove=1;
                    CancleFocus();
                }
            }

    if(blueflag==0)result=2;
    else if(redflag==0)result=1;
    else if(gamercanmove==0) result=!this->Gamer+1;
    return result;
}
void ChessWidget::FinishGame(int result)//结束游戏
{
    if(result!=0){

        this->GameStarted=0;
        char ftext[3][100]={"","（蓝色方）胜","（红色方）胜"};
        QString FinishText;
        if(result==1||result==2) FinishText="游戏结束：Gamer"+QString::number(this->GamersColor[result-1])+tr(ftext[result-1]);
        else FinishText=QString("游戏结束：Gamer%1超时3次判负").arg(result-3);
        emit signalText(FinishText);
        emit signaldisplaystatus(FinishText);
        emit signalappendLog(FinishText);
        if((result==1||result==2)&&this->isServer) emit signalServerupdate(this->GamersColor[result-1]+9);
        else if((result==3||result==4)&&this->isServer) emit signalServerupdate(!(result-3)+9);

    }
}

//落子逻辑
void ChessWidget::FocusAt(Position* now)
{
    now->focused=true;
    this->pfocus=now;
    FindTarget();
    update();
}
void ChessWidget::CancleFocus()
{
    if(this->pfocus==nullptr)return;
    this->pfocus->focused=false;
    pfocus=nullptr;
    ClearTarget();
    update();
}

void ChessWidget::FindTarget()
{
    TargetGeneraterRailway(this->pfocus);
    TargetGeneraterRoad(this->pfocus);
    TargetFilter();
}
void ChessWidget::ClearTarget()
{
    for(int i=0;i<5;i++){
        for(int j=0;j<12;j++){
            this->position[i][j].target=false;
        }
    }
    update();
}
void ChessWidget::TargetGeneraterRailway(Position* now)
{

    if(this->pfocus->man.number==1){
        if(now->iy==1||now->iy==5||now->iy==6||now->iy==10||((now->ix==0||now->ix==4)&&now->iy!=0&&now->iy!=11)){
            if(now->target==true) return;
            now->target=true;
        }
        if((now->iy==1||now->iy==5||now->iy==6||now->iy==10)&&(now->status==0||now==this->pfocus)){
            if(now->ix-1>=0)TargetGeneraterRailway(&(this->position[now->ix-1][now->iy]));
            if(now->ix+1<=4)TargetGeneraterRailway(&(this->position[now->ix+1][now->iy]));
        }
        if(((now->ix==0||now->ix==4)&&now->iy!=0&&now->iy!=11)&&(now->status==0||now==this->pfocus)){
            if(now->iy-1>=1)TargetGeneraterRailway(&(this->position[now->ix][now->iy-1]));
            if(now->iy+1<=10)TargetGeneraterRailway(&(this->position[now->ix][now->iy+1]));
        }
        if(now->ix==2&&(now->iy==5||now->iy==6)&&(now->status==0||now==this->pfocus)){
            TargetGeneraterRailway(&this->position[2][5]);
            TargetGeneraterRailway(&this->position[2][6]);
        }
    }else if(this->pfocus->man.number>1&&this->pfocus->man.number<11)
    {
        if(now->iy==1||now->iy==5||now->iy==6||now->iy==10){
            now->target=true;
            for(int i=now->ix-1;i>=0;i--){
                this->position[i][now->iy].target=true;
                if(this->position[i][now->iy].status!=0)break;

            }
            for(int i=now->ix+1;i<5;i++){
                this->position[i][now->iy].target=true;
                if(this->position[i][now->iy].status!=0)break;

            }
        }
        if(((now->ix==0||now->ix==4)&&now->iy!=0&&now->iy!=11)){
            now->target=true;
            for(int i=now->iy-1;i>=1;i--){
                this->position[now->ix][i].target=true;
                if(this->position[now->ix][i].status!=0)break;
            }

            for(int i=now->iy+1;i<11;i++){
                this->position[now->ix][i].target=true;
                if(this->position[now->ix][i].status!=0)break;
            }

        }
        if(now->ix==2&&(now->iy==5||now->iy==6)){
            this->position[2][5].target=true;
            this->position[2][6].target=true;
        }
    }
}
void ChessWidget::TargetGeneraterRoad(Position* now)
{
    if(now->iy!=0&&now->iy!=6)this->position[now->ix][now->iy-1].target=true;
    if(now->iy!=5&&now->iy!=11)this->position[now->ix][now->iy+1].target=true;
    if(now->ix!=0)this->position[now->ix-1][now->iy].target=true;
    if(now->ix!=4)this->position[now->ix+1][now->iy].target=true;
    if(now->iy>=1&&now->iy<=10){
        if(now->iy!=1&&now->iy!=6){
            if(now->ix!=0&&(now->ix+now->iy+(now->iy>5))%2)this->position[now->ix-1][now->iy-1].target=true;
            if(now->ix!=4&&(now->ix+now->iy+(now->iy>5))%2)this->position[now->ix+1][now->iy-1].target=true;
        }
        if(now->iy!=5&&now->iy!=10){
            if(now->ix!=0&&(now->ix+now->iy+(now->iy>5))%2)this->position[now->ix-1][now->iy+1].target=true;
            if(now->ix!=4&&(now->ix+now->iy+(now->iy>5))%2)this->position[now->ix+1][now->iy+1].target=true;
        }
    }
}
void ChessWidget::TargetFilter()
{
        for(int i=0;i<5;i++){
            for(int j=0;j<12;j++){
            //排除不动子
            if(this->pfocus->man.number==0||this->pfocus->man.number==11)ClearTarget();
            //排除未翻开子
            if(this->position[i][j].status==1)this->position[i][j].target=false;
            else if(this->position[i][j].status==2){
                //排除同色
                this->position[i][j].target=this->position[i][j].target&&(this->pfocus->man.color!=this->position[i][j].man.color);
                //排除大子
                if(this->position[i][j].man.number<10&&this->position[i][j].man.number>this->pfocus->man.number)this->position[i][j].target=false;
                //排除行营
                int EPosition[10][2]={{1,2},{3,2},{2,3},{1,4},{3,4},{1,7},{3,7},{2,8},{1,9},{3,9}};
                for(int i=0;i<10;i++){
                    if(this->position[EPosition[i][0]][EPosition[i][1]].status==2)this->position[EPosition[i][0]][EPosition[i][1]].target=false;
                }
                //排除地雷
                if(this->position[i][j].man.number==0&&this->pfocus->man.number!=1&&this->pfocus->man.number!=10)this->position[i][j].target=false;
                //排除有地雷时的军旗
                if(this->position[i][j].man.number==11){
                    bool is=false;
                    for(int p=0;p<5;p++){
                        for(int q=0;q<12;q++){
                            if(this->position[p][q].status!=0&&this->position[p][q].man.number==0&&this->position[p][q].man.color!=this->pfocus->man.color){
                                is=true;
                            }
                        }
                    }
                    if(is) this->position[i][j].target=false;
                }
            }
        }
    }
}
