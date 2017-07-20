#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include<QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtSerialPort/QSerialPort>
//! [0]
class workthread : public QThread
{
    Q_OBJECT

public:
    explicit workthread(QObject *parent = nullptr);
    ~workthread();

    void transaction(const QString &portName, const QByteArray &request);
    void run() Q_DECL_OVERRIDE;
    void Open();
    void Close();

signals:
    void response(const QByteArray &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    QString portName;
    QByteArray request;
    int waitTimeout;
    QMutex mutex;
    QWaitCondition cond;
    bool quit,portOpen;


//    QSerialPort serial;

//private slots:
//    void bytesWrittenSucceed(qint64 bytes);
};
//! [0]

#endif // MASTERTHREAD_H

