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
    void requestToSend_1(void *req); // 定义发送请求的信号

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void handleResponse(const QVariant &data);
    void keyPressEvent(QKeyEvent *event);
private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
