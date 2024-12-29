#include "responsethread.h"
#include <QDebug>
extern "C" {
    #include "client.h" // 这是你C语言逻辑代码的头文件
}
extern int client_fd;
extern ResponseThread* responseThread;

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
    qDebug() << "Response Thread started";
    receive_response();

    qDebug() << "Response Thread finished";
}
