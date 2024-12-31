#ifndef FRIENDCHAT_H
#define FRIENDCHAT_H

#include <QMainWindow>
#include <QTextBlock>
namespace Ui {
class FriendChat;
}

class FriendChat : public QMainWindow
{
    Q_OBJECT

public:
    explicit FriendChat(QWidget *parent = 0);
    ~FriendChat();
    QString getWindowTitle() const; // 添加获取窗口标题的方法
private slots:
    void on_sendButton_clicked();
protected:
    void closeEvent(QCloseEvent *event); // 正确：不需要 FriendChat::

private:
    Ui::FriendChat *ui;
};

#endif // FRIENDCHAT_H
