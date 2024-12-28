#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "responsethread.h"
#include "sendthread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void requestToSend(void *req); // 定义发送请求的信号
private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void handleResponse(const QVariant &data);
    void startResponseThread();
private:
    Ui::MainWindow *ui;
    ResponseThread *responseThread;
     SendThread *sendThread; // 发送线程作为成员变量
};


#endif // MAINWINDOW_H
