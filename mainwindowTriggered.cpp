#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"ipinput.h"
void MainWindow::on_Start_triggered()
{
    if(ui->game->isClient==0&&ui->game->isServer==0) ;//ui->game->StartGame();
    else if(ui->game->isServer)Server_SendMessage(1);
    else if(ui->game->isClient)Client_SendMessage(1);
}
void MainWindow::on_pushButton_clicked()
{
    if(ui->game->isClient==0&&ui->game->isServer==0) ;//ui->game->StartGame();
    else if(ui->game->isServer)Server_SendMessage(1);
    else if(ui->game->isClient)Client_SendMessage(1);
}

void MainWindow::on_CreateServer_triggered()
{
    Server_InitServer();
}
void MainWindow::on_ConnectServer_triggered()
{
    ipinput* Dialog=new ipinput(this);
    Dialog->setModal(true);
    Dialog->show();

}
void MainWindow::on_pushButton_2_clicked()
{
    if(ui->game->GameStarted==1)
    {
        if(ui->game->ActionTimer[2]+1>20){
            if(ui->game->isClient)Client_SendMessage(6);
            else if(ui->game->isServer)Server_SendMessage(6);
            ui->game->GameStarted=0;
            QString FinishText=QString("你(Gamer%1)认输了！").arg(ui->game->isClient);
            PrintInformation(FinishText);
            ui->textBrowser->setText(FinishText);
            ui->textBrowser_2->append(FinishText);
        }else{
            PrintInformation("20手后才能认输");
        }
    }
}
void MainWindow::on_Disconnect_triggered()
{
    if(ui->game->GameStarted==0){
        if(ui->game->isClient==1)
        {
            this->readWriteSocket->disconnectFromHost();
            ui->game->isClient=0;
        }else if(ui->game->isServer==1){
            Server_SendMessage(11);
        }
    }else PrintInformation("正在进行游戏，请勿关闭连接");
}
