#ifndef DELETEFRIENDDIALOG_H
#define DELETEFRIENDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <sendthread.h>
#include "logged.h"
extern "C" {
    #include "client.h" //C语言逻辑代码的头文件
}
extern SendThread* sendThread;
class DeleteFriendDialog : public QDialog {
    Q_OBJECT

public:
    explicit DeleteFriendDialog(logged *loggedWindow, QWidget *parent = nullptr); // 接收 logged 指针
    ~DeleteFriendDialog();

signals:
    void requestToSend(void *req); // 定义发送请求的信号
private slots:
    void onDeleteButtonClicked(); // 删除按钮点击槽函数

private:
    QLineEdit *friendNameInput;   // 输入好友名称的文本框
    QPushButton *deleteButton;    // 删除按钮
    logged * loggedwindow;
};

#endif // DELETEFRIENDDIALOG_H
