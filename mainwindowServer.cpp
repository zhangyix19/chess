#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::Server_InitServer()
{
    this->listenSocket =new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8888);
    this->readWriteSocket=nullptr;
    QString localHostName = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHostName);
    QString servIP;
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            servIP = address.toString();
    }
    QString servInfo = "本机地址:"+ servIP+":";
    servInfo+=QString::number(this->listenSocket->serverPort(),10);
    PrintInformation(servInfo);
    QObject::connect(this->listenSocket,&QTcpServer::newConnection,this,&MainWindow::Server_AcceptConnection);

}
void MainWindow::Server_AcceptConnection()
{
    if(this->readWriteSocket==nullptr){
        this->readWriteSocket =this->listenSocket->nextPendingConnection();
        QObject::connect(this->readWriteSocket,&QTcpSocket::readyRead,this,&MainWindow::Server_ReciveMessage);
        ui->game->isServer=1;
        QObject::connect(this->readWriteSocket,&QTcpSocket::disconnected,this,&MainWindow::Server_Disconnected);
        PrintInformation("Client连接成功");
    }
}
void MainWindow::Server_Disconnected()
{
    if(this->readWriteSocket!=nullptr) this->readWriteSocket=nullptr;
    ui->game->isServer=0;
    PrintInformation("已断开连接");
}
/*
 *status
 *0 更新
 *1 请求开始游戏
 *2 同意开始游戏
 *3 拒绝开始游戏
 *4 server发送游戏数据
 *5 client发送游戏动作
 *6 client认输
 *7 server发送开始游戏数据
 *8 蓝红色
 *9 0胜
 *10 1胜
 *11 关闭连接
 */
void MainWindow::Server_ReciveMessage()
{
    if(this->readWriteSocket==nullptr)return;
    while(this->readWriteSocket->bytesAvailable()>=sizeof(int)+sizeof(clickdata)){
        char buf[4096];
        this->readWriteSocket->read(buf,sizeof(int)+sizeof(clickdata));
        int status;
        memcpy(&status,buf,sizeof(int));
        if(!ui->game->isGameStarted()){

            if(status==1){
                int ret = QMessageBox::warning(0, tr("Server"), tr("Client请求开始游戏"), QMessageBox::Yes | QMessageBox::No);
                if (ret == QMessageBox::Yes) {
                    ui->game->StartGame();
                    Server_SendMessage(7);
                } else {
                    Server_SendMessage(3);
                }
            }else if(status==2){
                ui->game->StartGame();
                PrintInformation("游戏开始");
                Server_SendMessage(7);
            }else if(status==3){
                PrintInformation("对方拒绝了游戏请求");
            }
        }else{
            if(status==5){
                clickdata data;
                memcpy(&data,buf+sizeof(int),sizeof(struct clickdata));
                if(data.lorr==0){
                    ui->game->Leftclick(data.x,data.y);
                }else if(data.lorr==1){
                    ui->game->CancleFocus();
                }
                Server_SendMessage(4);
            }else if(status==6){
                ui->game->GameStarted=0;
                QString FinishText="对方认输，你(Gamer0)胜利了";
                PrintInformation(FinishText);
                ui->textBrowser->setText(FinishText);
                ui->textBrowser_2->append(FinishText);
            }
        }
    }
}
void MainWindow::Server_SendMessage(int status)
{
    package pak;
    pak.status=status;
    GAME gamedata=ui->game->GetGamedata();
    memcpy(pak.data, &gamedata, sizeof(struct GAME));
    QByteArray data;
    data.resize(sizeof(struct GAME)+sizeof(int));
    memcpy(data.data(), &pak, sizeof(struct GAME)+sizeof(int));
    this->readWriteSocket->write(data);
}
