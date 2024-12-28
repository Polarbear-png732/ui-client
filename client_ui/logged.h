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
    explicit logged(QWidget *parent = nullptr, QWidget *loginWindow = nullptr);
    ~logged();
protected:
    void closeEvent(QCloseEvent *event) override; // 处理关闭事件

private:
    Ui::logged *ui;
    QWidget *loginWindow;  // 存储登录窗口指针
};

#endif // LOGGED_H
