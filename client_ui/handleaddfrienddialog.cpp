#include "handleaddfrienddialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <sendthread.h>
extern SendThread* sendThread;
extern "C" {
    #include "client.h"  // C语言逻辑代码的头文件
}

HandleFriendDialog::HandleFriendDialog( const QString &message,QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("处理好友请求");
    setFixedSize(300, 200);

    // 创建显示好友名字的标签
    friendNameLabel = new QLabel(this);
    friendNameLabel->setText( message);
    friendNameLabel->setAlignment(Qt::AlignCenter);

    // 创建按钮
    QPushButton *acceptButton = new QPushButton("接受", this);
    QPushButton *rejectButton = new QPushButton("拒绝", this);
    acceptButton->setMaximumWidth(150);  // 设置最大宽度
    rejectButton->setMaximumWidth(150);
    acceptButton->setMinimumWidth(100);  // 设置最小宽度
    rejectButton->setMinimumWidth(100);
    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 设置 friendNameLabel 居中
    layout->addWidget(friendNameLabel, 0, Qt::AlignCenter);

    // 设置按钮居中
    layout->addWidget(acceptButton, 0, Qt::AlignCenter);
    layout->addWidget(rejectButton, 0, Qt::AlignCenter);

    // 布局调整
    setLayout(layout);

    // 连接按钮点击信号
    connect(acceptButton, &QPushButton::clicked, this, &HandleFriendDialog::acceptFriendRequest);
    connect(rejectButton, &QPushButton::clicked, this, &HandleFriendDialog::rejectFriendRequest);
    connect(this, &HandleFriendDialog::requestToSend, sendThread, &SendThread::sendRequest);
}

void HandleFriendDialog::acceptFriendRequest() {
    // 构建接受好友请求的数据
    HandleFriendRequest *request = (HandleFriendRequest *)malloc(sizeof(HandleFriendRequest));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_HANDELE_ADD);
    int action=1;
    request->action = htonl(action);
    request->length = htonl(sizeof(HandleFriendRequest));

    strncpy(request->session_token, session_token, sizeof(session_token) - 1);
    request->session_token[sizeof(session_token) - 1] = '\0';
    QString username =friendNameLabel->text();
    strncpy(request->friend_username, username.toStdString().c_str(), sizeof(request->friend_username) - 1);
    request->friend_username[sizeof(request->friend_username) - 1] = '\0';  // 确保字符串以 null 结尾
    emit requestToSend(request);
    // 关闭对话框
    accept();
}

void HandleFriendDialog::rejectFriendRequest() {
    // 构建拒绝好友请求的数据
    HandleFriendRequest *request = (HandleFriendRequest *)malloc(sizeof(HandleFriendRequest));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_HANDELE_ADD);
    int action=0;
    request->action = htonl(action);
    request->length = htonl(sizeof(HandleFriendRequest));

    strncpy(request->session_token, session_token, sizeof(session_token) - 1);
    request->session_token[sizeof(session_token) - 1] = '\0';
    QString username = friendNameLabel->text();
    strncpy(request->friend_username, username.toStdString().c_str(), sizeof(request->friend_username) - 1);
    request->friend_username[sizeof(request->friend_username) - 1] = '\0';  // 确保字符串以 null 结尾
    emit requestToSend(request);
    // 关闭对话框
    accept();
}
