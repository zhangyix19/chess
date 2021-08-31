#ifndef IPINPUT_H
#define IPINPUT_H

#include <QDialog>
#include <QHostAddress>
#include<QMessageBox>
namespace Ui {
class ipinput;
}

class ipinput : public QDialog
{
    Q_OBJECT

public:
    explicit ipinput(QWidget *parent = nullptr);
    ~ipinput();
signals:
    void signalip(QString,int);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::ipinput *ui;
};

#endif // IPINPUT_H
