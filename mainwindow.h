#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include<QString>
#include<QTime>
#include<QTimer>
#include <QtNetwork>
#include <QtDebug>
#include <QKeyEvent>
#include <QTextStream>
#include <QMessageBox>
#include <QLCDNumber>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent * event);
    ~MainWindow();
public slots:
    void Server_InitServer();
    void Server_Disconnected();
    void Server_AcceptConnection();
    void Server_ReciveMessage();
    void Server_SendMessage(int status=0);
    void Client_ConnectHost(QString,int);
    void ClientConnected();
    void Client_Disconnected();
    void Client_ReciveMessage();
    void Client_SendMessage(int status=0,int x=0,int y=0,int lorr=0);
    //void Client_SendGameData();
    void PrintInformation(QString);
    void Client_ErrorConnected();
private slots:
    void on_Start_triggered();
    void on_CreateServer_triggered();
    void on_ConnectServer_triggered();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_Disconnect_triggered();

signals:

private:

    QTimer* Timer;
    Ui::MainWindow *ui;
    QTcpServer  *listenSocket;
    QTcpSocket  *readWriteSocket;
    QTcpSocket  *dataSocket;
    int CSMode();

};
#endif // MAINWINDOW_H
