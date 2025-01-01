#include "friendchat.h"
#include "ui_friendchat.h"
#include "QString"
#include "QDebug"
#include <QShortcut>
extern "C" {
    #include "client.h"  // C语言逻辑代码的头文件
}
FriendChat::FriendChat(QWidget *parent,friendItem* item) :
    QMainWindow(parent),
    ui(new Ui::FriendChat),
    item(item)
{
    ui->setupUi(this);
    ui->message->setReadOnly(true);  // 将 QTextEdit 设置为只读
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    this->loadText();
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

    item->addSentMessage(message);//消息加入item的消息记录中
    QStringList history = item->getMessageHistory();
    for (const QString &msg : history) {
        qDebug() << msg;  // 输出消息记录
    }

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
    ui->inputmsg->clear();
    this->SentToText(message);
}

//收到消息，添加到聊天框
void FriendChat::MsgRcvd(const QString msg){
    this->RecvdToText(msg);
    item->addReceivedMessage(msg);
}

//向聊天框添加收到多消息，绿色
void  FriendChat::RecvdToText(const QString msg){
    QString formattedMsg = QString(
        "<div style='margin: 10px; color: green;'>"
        "<strong></strong> %1"
        "</div>"
    ).arg(msg);
    ui->message->append(formattedMsg);
}
//向聊天框添加发送的消息，蓝色
void  FriendChat::SentToText(const QString msg){
    QString formattedMessage = QString(
        "<div style='margin: 10px; color: blue;'>"
        "<strong></strong> %1"
        "</div>"
    ).arg(msg);
    ui->message->append(formattedMessage);
}

void FriendChat::loadText() {
    QStringList history = item->getMessageHistory();
    for (const QString &msg : history) {
        if (msg.startsWith("发送:")) {
            // 提取消息内容（去掉前缀 "发送:"）
            QString message = msg.mid(3).trimmed();
            SentToText(message);
        } else if (msg.startsWith("接收:")) {
            // 提取消息内容（去掉前缀 "接收:"）
            QString message = msg.mid(3).trimmed();
            RecvdToText(message);
        } else {

            qDebug() << "未知消息格式:" << msg;
        }
    }
}
