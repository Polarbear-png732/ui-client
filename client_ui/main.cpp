#include "mainwindow.h"
#include <QApplication>
#include <QFile>

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
    return a.exec(); // 使用正确的对象名称 a
}
