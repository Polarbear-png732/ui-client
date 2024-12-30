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
    explicit logged(QWidget *parent = nullptr);
    ~logged();
    void handleResponse(const QVariant &data);
protected:
    void closeEvent(QCloseEvent *event) override; // 处理关闭事件
signals:
    void requestToSend(void *req); // 定义发送请求的信号
private slots:
    void updateFriendList();
    void on_addfriend_clicked();

    void on_friendReq_clicked();

private:
    Ui::logged *ui;
    QMainWindow *loginWindow;  // 存储登录窗口指针

};

#endif // LOGGED_H
