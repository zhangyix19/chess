#include "ipinput.h"
#include "ui_ipinput.h"
#include"mainwindow.h"
ipinput::ipinput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ipinput)
{
    ui->setupUi(this);
    ui->lineEdit->setInputMask("000.000.000.000:00000;");
    MainWindow* p= (MainWindow*) this->parent();
    QObject::connect(this,&ipinput::signalip,p,&MainWindow::Client_ConnectHost);
}
ipinput::~ipinput()
{
    delete ui;
}

void ipinput::on_buttonBox_accepted()
{
    QStringList strlist=ui->lineEdit->text().split(":");
    QString ip=strlist[0];
    int port=strlist[1].toInt();
    QHostAddress* temp=new QHostAddress;
    if(temp->setAddress(ip)==QHostAddress::Null){
        QMessageBox::information(0,tr("ipinput"),"ip地址有误",QMessageBox::Yes);
    }else{
        emit signalip(ip,port);
    }
    delete temp;
}

