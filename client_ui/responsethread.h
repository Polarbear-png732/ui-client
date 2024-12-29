#ifndef RESPONSETHREAD_H
#define RESPONSETHREAD_H

#include <QThread>
#include <QObject>
#include <QString>
#include "client.h"
extern "C" {
    void *receive_response(); // 声明 C 函数
}


class ResponseThread : public QThread
{

    Q_OBJECT

public:
    explicit ResponseThread(QObject *parent = nullptr);
    ~ResponseThread();

protected:
    void run() override;
signals:
    void responseReceived(const QVariant &data); // 定义信号以通知主线程
};



#endif // RESPONSETHREAD_H
