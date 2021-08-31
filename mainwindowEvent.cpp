#include "mainwindow.h"
#include "ui_mainwindow.h"
void MainWindow::closeEvent(QCloseEvent * event) {
    if(ui->game->GameStarted==0){
        int ret = QMessageBox::warning(0, tr("MainWindow"), tr("Do you really want to exit?"), QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            event->accept();
        } else {
            event->ignore();
        }
    }else{
        PrintInformation("正在进行游戏，请勿关闭");
        event->ignore();
    }
}

