#ifndef HANDLEFREINDREQUESTDIALOG_H
#define HANDLEFREINDREQUESTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <sendthread.h>
extern "C" {
    #include "client.h"  // C语言逻辑代码的头文件
}
extern SendThread* sendThread;

class HandleFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HandleFriendDialog( const QString &message,QWidget *parent = nullptr);

signals:
    void requestToSend(void *req);  // 定义发送请求的信号

private slots:
    void acceptFriendRequest();
    void rejectFriendRequest();

private:
    QLabel *friendNameLabel;  // 显示好友名字的标签
};

#endif // HANDLEFREINDREQUESTDIALOG_H
