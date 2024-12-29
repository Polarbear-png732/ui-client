#ifndef LOGGED_H
#define LOGGED_H

#include <QMainWindow>

namespace Ui {
class logged;
}

class logged : public QMainWindow
{
    Q_OBJECT

public:
    explicit logged(QWidget *parent = nullptr, QMainWindow *loginWindow = nullptr);
    ~logged();
protected:
    void closeEvent(QCloseEvent *event) override; // 处理关闭事件

private:
    Ui::logged *ui;
    QMainWindow *loginWindow;  // 存储登录窗口指针
};

#endif // LOGGED_H
