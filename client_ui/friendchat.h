#ifndef FRIENDCHAT_H
#define FRIENDCHAT_H
#include "frienditem.h"
#include <QMainWindow>
#include <QTextBlock>
extern "C" {
    #include "client.h"  // C语言逻辑代码的头文件
}
namespace Ui {
class FriendChat;
}

class FriendChat : public QMainWindow
{
    Q_OBJECT

public:
    explicit FriendChat(QWidget *parent = nullptr, friendItem* item = nullptr);
    ~FriendChat();
    QString getWindowTitle() const; // 添加获取窗口标题的方法
    QString getMessage()const;
    void MsgRcvd(const QString msg);
    void RecvdToText(const QString msg);
    void SentToText(const QString msg);
    void loadText();
private slots:
    void on_sendButton_clicked();

protected:
    void closeEvent(QCloseEvent *event); // 正确：不需要 FriendChat::
signals:
    void requestToSend(void *req); // 定义发送请求的信号
private:
    Ui::FriendChat *ui;
    friendItem* item;
};

#endif // FRIENDCHAT_H
