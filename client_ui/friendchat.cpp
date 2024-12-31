#include "friendchat.h"
#include "ui_friendchat.h"
#include "QString"
extern "C" {
    #include "client.h"  // C语言逻辑代码的头文件
}
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
QString FriendChat::getMessage() const
{
    return  ui->inputmsg->toPlainText(); // 获取输入的消息;
}


void FriendChat::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event); // 保留基类行为
    if (parentWidget()) {
        parentWidget()->show(); // 关闭注册窗口时显示父窗口
    }
}

void FriendChat::on_sendButton_clicked()
{
    QString title=this->getWindowTitle();
    QStringList parts = title.split('('); // 根据 '(' 拆分字符串
    QString name = parts[0]; // 第一个部分就是名字
    QString message=this->getMessage();
    PrivateMessage *request = (PrivateMessage*)malloc(sizeof(PrivateMessage));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_PRIVATE_MESSAGE);
    request->length = htonl(sizeof(PrivateMessage));
    strncpy(request->session_token, session_token, TOKEN_LEN - 1);
    request->session_token[TOKEN_LEN - 1] = '\0';

    strcpy(request->receiver_username,name.toStdString().c_str());
    strcpy(request->message,message.toStdString().c_str());
    emit  requestToSend(request);
}
