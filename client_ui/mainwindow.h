#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "responsethread.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void handleResponse(const QString &message);
    void startResponseThread();  // 延迟启动线程
private:
    Ui::MainWindow *ui;
    ResponseThread *responseThread; // 添加线程对象
};


#endif // MAINWINDOW_H
