#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTimer>
#include <iostream>
#include "logged.h"
#include "sendthread.h"
#include "responsethread.h"
#include <QDebug>
extern "C" {
    #include "client.h" // 这是你C语言逻辑代码的头文件
}
extern int client_fd;
ResponseThread* responseThread=nullptr;
SendThread* sendThread=nullptr;
pthread_mutex_t mutex;
pthread_cond_t cond;

void init_client()
{
    struct sockaddr_in server_addr;

    // 创建主套接字
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0)
    {
        perror("创建套接字失败");
        exit(1);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    // 连接服务器
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("连接服务器失败");
        close(client_fd);
        exit(1);
    }
        qDebug() << "Client FD while create:" << QString::number(client_fd);
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // QApplication 的实例名为 a

    start_client();

    QFile file(":/QSS/style.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(QLatin1String(file.readAll())); // 使用正确的对象名称 a
        file.close();
    }
    int logflag=0;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    sendThread = new SendThread(&a);
    sendThread->start();
    responseThread = new ResponseThread(&a); // 父对象是 QCoreApplication
    responseThread->start();
    MainWindow w;
    w.show();

    qDebug() << "Client FD:" << QString::number(client_fd);





//    if (responseThread->isRunning()) {
//        responseThread->requestInterruption(); // 请求中断
//        responseThread->quit();               // 退出线程事件循环
//        responseThread->wait();               // 等待线程退出
//    }
//    if (sendThread) {
//        sendThread->stop(); // 自定义的停止方法，设置标志并唤醒线程
//        sendThread->wait(); // 等待线程安全退出
//        delete sendThread;
//    }
//    logged log;
//    log.show();

    return a.exec(); // 使用正确的对象名称 a
}



