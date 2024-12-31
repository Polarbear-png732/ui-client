#include "friendchat.h"
#include "ui_friendchat.h"
#include "QString"
FriendChat::FriendChat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FriendChat)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
}

FriendChat::~FriendChat()
{
    delete ui;
}

QString FriendChat::getWindowTitle() const
{
    return this->windowTitle(); // 返回窗口标题
}

void FriendChat::on_sendButton_clicked()
{
    QString title=this->getWindowTitle();

}

void FriendChat::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event); // 保留基类行为
    if (parentWidget()) {
        parentWidget()->show(); // 关闭注册窗口时显示父窗口
    }
}
