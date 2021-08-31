#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"ipinput.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    Timer=new QTimer;
    Timer->setInterval(1000);
    Timer->start(1000);
    QWidget::connect(ui->game, &ChessWidget::signalText,this, &MainWindow::PrintInformation);
    QWidget::connect(ui->game, &ChessWidget::signaldisplaystatus, ui->textBrowser,&QTextBrowser::setText);
    QWidget::connect(ui->game, &ChessWidget::signalappendLog, ui->textBrowser_2,&QTextBrowser::append);
    void(QLCDNumber::*QLCDNumberdisplayFunction)(int)=&QLCDNumber::display;
    QWidget::connect(ui->game, &ChessWidget::signaldisplayRamainTime, ui->lcdNumber,QLCDNumberdisplayFunction);
    QWidget::connect(Timer,&QTimer::timeout,ui->game,&ChessWidget::ChessTimer);
    QWidget::connect(ui->game,&ChessWidget::signalClientClick,this,&MainWindow::Client_SendMessage);
    QWidget::connect(ui->game,&ChessWidget::signalServerupdate,this,&MainWindow::Server_SendMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::PrintInformation(QString str)
{
    QString title;
    if(ui->game->isClient==1)title="Client(Gamer1)";
    else if(ui->game->isServer==1) title="Sever(Gamer0)";
    else title="MainWindow";
    QMessageBox::information(this,title,str,QMessageBox::Yes);
}












