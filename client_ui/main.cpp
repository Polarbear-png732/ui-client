#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTimer>
#include <iostream>
extern "C" {
    #include "client.h" // 这是你C语言逻辑代码的头文件
}
extern int client_fd;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // QApplication 的实例名为 a
    QFile file(":/QSS/style.qss");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        a.setStyleSheet(QLatin1String(file.readAll())); // 使用正确的对象名称 a
        file.close();
    }
    MainWindow w;
    w.show();
    QTimer::singleShot(0, []() {
        start_client(); // 启动C语言客户端
    });
    std::cout << "Client FD: " << client_fd << std::endl; // 打印套接字描述符，测试是否能访问
    return a.exec(); // 使用正确的对象名称 a
}
