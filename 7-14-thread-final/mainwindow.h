#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include<QTimer>

#include <QByteArray>
#include "workthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_beginButton_clicked();                 //开始读写

    void on_endButton_clicked();                   //暂停读写

    void on_freshButton_clicked();

    void on_openButton_clicked();

    void on_closeButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::MainWindow *ui;

//    QSerialPort *serial;
    QTimer *time_clock;                                     //定时器
    void StringToHex(QString str, QByteArray &senddata);   //转换为16进制
    char ConvertHexChar(char ch);
    QByteArray senddata;                                   //需要发送的字节数组
    QString temp;
    double toangle(const QString &c);                     //将接收的数据转为角度信息
//    QSerialPort serial;
    workthread athread;
    bool isRunning;                                      //指示定时器工作状态
    void timeOut();
    void showResponse(const QByteArray &s);
     void processError(const QString &s);
     QSerialPort serial;
};

#endif // MAINWINDOW_H
