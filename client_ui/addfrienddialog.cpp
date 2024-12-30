#include "addfrienddialog.h"

AddFriendDialog::AddFriendDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("添加好友");
    setFixedSize(300, 150);
    // 创建控件
    QLabel *label = new QLabel("输入好友用户名:", this);
    friendInput = new QLineEdit(this);
    QPushButton *sendButton = new QPushButton("发送", this);

    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(friendInput);
    layout->addWidget(sendButton);

    setLayout(layout);
    connect(this, &AddFriendDialog::requestToSend, sendThread, &SendThread::sendRequest);
    connect(sendButton, &QPushButton::clicked, this, &AddFriendDialog::sendFriendRequest);
}





