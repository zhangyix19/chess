#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::Client_ConnectHost(QString ip,int port)
{
    if( port>65535){
        PrintInformation("无效ip");
        return;
    }
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress(ip),port);
    QObject::connect(this->readWriteSocket,&QTcpSocket::connected,this,&MainWindow::ClientConnected);
    QObject::connect(this->readWriteSocket,&QTcpSocket::readyRead,this,&MainWindow::Client_ReciveMessage);
    QObject::connect(this->readWriteSocket,&QTcpSocket::disconnected,this,&MainWindow::Client_Disconnected);
    QObject::connect(this->readWriteSocket,&QTcpSocket::errorOccurred,this,&MainWindow::Client_ErrorConnected);
    bool sts=this->readWriteSocket->waitForConnected(3000);
    if(sts==0 ){
        PrintInformation("无效ip");
        delete this->readWriteSocket;
        ui->game->isClient=0;
    }
}
void MainWindow::ClientConnected(){
    PrintInformation("Server连接成功");
    ui->game->isClient=1;

}
void MainWindow::Client_ErrorConnected(){
    ;
}
void MainWindow::Client_Disconnected()
{
    if(this->readWriteSocket!=nullptr) this->readWriteSocket=nullptr;
    ui->game->isClient=0;
    PrintInformation("已断开连接");
}
/*
 *status
 *1 请求开始游戏
 *2 同意开始游戏
 *3 拒绝开始游戏
 *4 server发送游戏数据
 *5 client发送游戏动作
 *6 认输
 *7 server发送开始游戏数据
 */
void MainWindow::Client_ReciveMessage()
{
    if(this->readWriteSocket!=nullptr)
    {
        while(this->readWriteSocket->bytesAvailable()>=sizeof(int)+sizeof(GAME)){
            char buf[4096];
            this->readWriteSocket->read(buf,sizeof(int)+sizeof(GAME));
            int status;
            memcpy(&status,buf,sizeof(int));
            if(ui->game->isGameStarted()!=1){
                if(status==1){
                    int ret = QMessageBox::warning(0, tr("Client"), tr("Server请求开始游戏"), QMessageBox::Yes | QMessageBox::No);
                    if (ret == QMessageBox::Yes) {
                        Client_SendMessage(2);

                    } else {
                        Client_SendMessage(3);
                    }
                }else if(status==2){
                    GAME data;
                    memcpy(&data,buf+sizeof(int),sizeof(struct GAME));
                    ui->game->CoverGamedata(data);

                }else if(status==3){
                    PrintInformation("对方拒绝了游戏请求");
                }else if(status==7){
                    PrintInformation("游戏开始");
                    GAME data;
                    memcpy(&data,buf+sizeof(int),sizeof(struct GAME));
                    ui->game->CoverGamedata(data);

                }
            }else{
                if(status==4){
                    GAME data;
                    memcpy(&data,buf+sizeof(int),sizeof(struct GAME));
                    ui->game->CoverGamedata(data);

                }else if(status==8){
                    GAME data;
                    memcpy(&data,buf+sizeof(int),sizeof(struct GAME));
                    ui->game->CoverGamedata(data);
                    PrintInformation(QString("Gamer%1为%2！\n").arg(ui->game->Gamer).arg(ui->game->strColor(ui->game->Gamer))+QString("Gamer%1为%2！").arg(!ui->game->Gamer).arg(ui->game->strColor(!ui->game->Gamer)));
                }
                if(status==6){
                    ui->game->GameStarted=0;
                    QString FinishText="对方认输，你(Gamer1)胜利了";
                    PrintInformation(FinishText);
                    ui->textBrowser->setText(FinishText);
                    ui->textBrowser_2->append(FinishText);
                }
            }
            if(status==9||status==10){
                ui->game->GameStarted=0;
                PrintInformation(QString("Gamer%1胜").arg(status-9));
                ui->textBrowser->setText(QString("Gamer%1胜").arg(status-9));

            }else if(status==11){
                readWriteSocket->readAll();
                this->readWriteSocket->disconnectFromHost();
                ui->game->isClient=0;
                break;
            }
            if(ui->game->isGameStarted()==1){
                ui->game->updateClientTimer();
                ui->game->update();
                ui->game->UpdateStatus();
            }
        }
    }
}

void MainWindow::Client_SendMessage(int status,int x,int y,int lorr)
{
    package pak;
    pak.status=status;
    clickdata clkdata;
    clkdata.x=x;
    clkdata.y=y;
    clkdata.lorr=lorr;
    memcpy(pak.data, &clkdata, sizeof(struct clickdata));
    QByteArray data;
    data.resize(sizeof(int)+sizeof(struct clickdata));
    memcpy(data.data(), &pak, sizeof(int)+sizeof(struct clickdata));
    this->readWriteSocket->write(data);
}
