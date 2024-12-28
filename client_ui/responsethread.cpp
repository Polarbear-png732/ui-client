#include "responsethread.h"
#include <QDebug>
#include "client.h"
ResponseThread::ResponseThread(QObject *parent)
    : QThread(parent)
{
}

ResponseThread::~ResponseThread()
{
    requestInterruption(); // 请求线程中断
    quit();                // 退出线程事件循环
    wait();                // 等待线程完全退出
}

void ResponseThread::run()
{
    qDebug() << "Thread started";
    connect(&responseHandler, &ResponseHandler::messageReceived, this, &ResponseThread::responseReceived, Qt::QueuedConnection);
    // 调用 C 函数，传递 responseHandler 的指针，需要进行类型转换
    receive_response(static_cast<void*>(&responseHandler));

    qDebug() << "Thread finished";
}
