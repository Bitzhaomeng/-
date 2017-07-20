#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"
#include "QDebug"

//#include <QSerialPort>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  ,time_clock(new QTimer())
  , isRunning(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//寻找可用的串口  
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
       {
//           QSerialPort serial;
           serial.setPort(info);
           if(serial.open(QIODevice::ReadWrite))
           {
               ui->port->addItem(serial.portName());
               serial.close();
           }
       }

        //建立定时器
     connect(time_clock,&QTimer::timeout,this,&MainWindow::timeOut);//定时发送信号
     connect(&athread, &workthread::response, this, &MainWindow::showResponse);
     connect(&athread, &workthread::error, this, &MainWindow::processError);
  //   connect(&athread, &workthread::timeout, this, &MainWindow::processTimeout);

//要发送的命令
QString str="6804000408";
 StringToHex(str,senddata);
 ui->closeButton->setEnabled(false);
 ui->endButton->setEnabled(false);
  ui->beginButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_beginButton_clicked()            //开始数据读写
{
    if (isRunning == false)
    {
        time_clock->start(100);
        qDebug()<<"time start";
        isRunning = true;
    }
    ui->beginButton->setEnabled(false);
    ui->endButton->setEnabled(true);
    ui->state->setText("串口连接");                    //显示串口连接状态
     ui->freshButton->setEnabled(false);
//     ui->port->setEnabled(false);
     ui->closeButton->setEnabled(false);
     ui->openButton->setEnabled(false);
}

void MainWindow::on_endButton_clicked()                //暂停读写
{
    if (isRunning == true)
    {
        time_clock->stop();
        isRunning = false;
    }
    ui->beginButton->setEnabled(true);
    ui->endButton->setEnabled(false);
     ui->freshButton->setEnabled(false);
     ui->port->setEnabled(false);
      ui->closeButton->setEnabled(true);

}

void MainWindow::StringToHex(QString str, QByteArray &senddata)            //字符串转为16进制字节
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}
char MainWindow::ConvertHexChar(char ch)                 //转16进制中的函数
{
    if((ch >= '0') && (ch <= '9'))
            return ch-0x30;
        else if((ch >= 'A') && (ch <= 'F'))
            return ch-'A'+10;
        else if((ch >= 'a') && (ch <= 'f'))
            return ch-'a'+10;
        else return (-1);
}

double MainWindow::toangle(const QString &c)          //接收信号转为角度信息
{
    double answer;
QString sign=c.mid(0,1);                    //截取符号位，整数位和小数位
QString round1=c.mid(1,1);
QString round2=c.mid(2,2);
QString decimal=c.mid(4,2);
 answer=100*round1.toDouble()+round2.toDouble()+0.01*decimal.toDouble(); //数据整合为实际角度
if(sign.toInt()==1)
{
    answer=-1*answer;               //符号位为1时取负
}
return answer;

}
void MainWindow::timeOut()
{
  athread.transaction(ui->port->currentText(),senddata);
}
void MainWindow::showResponse(const QByteArray &s)
{
    temp= s.toHex();             //接收信号转16进制

    QString a,b,c,P;
    a=temp.mid(8,6);                     //截取接收信号 pitch roll head
    b=temp.mid(14,6);
     c=temp.mid(20,6);
     ui->pitch->setText(P.setNum(toangle(a)).append("°"));      //最终转为角度信息显示
     ui->roll->setText(P.setNum(toangle(b)).append("°"));
     ui->head->setText(P.setNum(toangle(c)).append("°"));
     ui->label->setText(temp);
}

void MainWindow::processError(const QString &s)
{
    ui->state->setText(s);
    ui->freshButton->setEnabled(true);
    ui->endButton->setEnabled(false);
}

void MainWindow::on_freshButton_clicked()
{
     ui->port->clear();
     foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
//            QSerialPort serial;
            serial.setPort(info);
            if(serial.open(QIODevice::ReadWrite))
            {
                ui->port->addItem(serial.portName());
                serial.close();
            }
        }
    ui->beginButton->setEnabled(false);
    ui->port->setEnabled(true);

}

void MainWindow::on_openButton_clicked()
{
      athread.Open();
      ui->closeButton->setEnabled(true);
      ui->port->setEnabled(false);
       ui->beginButton->setEnabled(true);
       ui->openButton->setEnabled(false);
}

void MainWindow::on_closeButton_clicked()
{
     athread.Close();
    ui->state->setText("串口断开");
    ui->beginButton->setEnabled(false);
    ui->openButton->setEnabled(true);
    ui->closeButton->setEnabled(false);
    ui->freshButton->setEnabled(true);
    athread.transaction(ui->port->currentText(),senddata);
}

void MainWindow::on_exitButton_clicked()
{
     MainWindow::close();
}
