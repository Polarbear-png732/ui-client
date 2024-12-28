#ifndef SENDTHREAD_H
#define SENDTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <client.h>

class SendThread : public QThread {
    Q_OBJECT

public:
    explicit SendThread(QObject *parent = nullptr); // 只声明构造函数
    ~SendThread() override;

    void stop();                    // 停止线程
    void sendRequest(void *req);     // 发送请求

protected:
    void run() override;             // 重写线程执行函数

private:
    QMutex mutex;                    // 保护共享资源
    QWaitCondition condition;        // 用于线程等待和唤醒
    QQueue<void *> pendingRequests;  // 请求队列
    bool stopFlag;                   // 停止标志
};

#endif // SENDTHREAD_H
