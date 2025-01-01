#include "logged.h"
#include "ui_logged.h"
#include <QListWidget>
#include <QCloseEvent>
#include "frienditem.h"
#include "responsethread.h"
#include "sendthread.h"
#include <QDebug>
#include <QMessageBox>
#include "addfrienddialog.h"
#include "handleaddfrienddialog.h"
#include <QTimer>
#include "friendreqlist.h"
#include "friendchat.h"
#include "frienditem.h"
#include "sendthread.h"
#include "messagedispatcher.h"


extern "C" {
    #include "client.h" // 这是你C语言逻辑代码的头文件
}
extern ResponseThread* responseThread;
extern SendThread* sendThread;
extern GroupInfo groups[MAX_FRIENDS];

extern int groupCount;
logged::logged(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::logged)
{
        ui->setupUi(this);
        sharedList.clear(); // 清空 sharedList
    for (int i = 0; i < 5; i++) { // 假设显示空的占位项
             friendItem *friItem = new friendItem();
             QListWidgetItem* m_Item = new QListWidgetItem(ui->listWidget);
             m_Item->setSizeHint(QSize(300, 90));
             ui->listWidget->setItemWidget(m_Item, friItem);
        }
    ui->headimg->setStyleSheet(
        "QLabel {"
        "    border-radius: 25px;" // 半径为宽或高的一半
        "    border: 2px solid gray;" // 可选，添加边框
        "    border-image: url(:/image/poalrbear.png) 0 0 0 0 stretch stretch;" // 设置图片
        "}"
    );


       ui->listWidget->setStyleSheet("QListWidget::item:hover {"
                                      "background-color: rgb(200,200,200);"
                                      "}");
       sharedList.clear();
       this->setProperty("sharedList", QVariant::fromValue(sharedList));

}

logged::~logged()
{
    delete ui;
}

// 在关闭 logged 窗口时重新显示登录窗口
void logged::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event); // 保留基类行为
    if (parentWidget()) {
        parentWidget()->show(); // 关闭注册窗口时显示父窗口
    }
}



void logged::handleResponse(const QVariant &data)
{

    if (data.type() == QVariant::String) {
        qDebug() << "QString type:" << data;
        // 如果数据是 QString 类型
        const QString dataString = data.toString();
        char* message = dataString.toUtf8().data(); // 或者 dataString.toStdString().c_str();

        if (strncmp(message, "好友", 6) == 0) { // "好友" 是 6 个字节
            parseFriendList(message);
            QMetaObject::invokeMethod(this, "updateFriendList", Qt::QueuedConnection);
           }
        else if(strncmp(message, "群聊", 6) == 0){
            parseGroupInfo(message,groups,&groupsCount);
        }
        else if(strncmp(message,"添加成功",12)==0){
             char name[32] = {0}; // 用于存储好友名字
             char onoff[8] = {0}; // 用于存储在线状态

            // 查找名字部分的起始位置（"添加成功"后）
               const char *nameStart = message + 12; // 跳过 "添加成功" 的部分
               // 查找在线状态部分的起始位置（"不在线" 或 "在线"）
               const char *statusStart = strstr(nameStart, "在线");
               if (!statusStart) {
                   statusStart = strstr(nameStart, "不在线");
               }
               if (statusStart) {
                   // 提取名字部分
                   size_t nameLength = statusStart - nameStart;
                   strncpy(name, nameStart, nameLength);
                   name[nameLength] = '\0'; // 确保名字字符串以 null 结尾

                   // 根据状态赋值
                   if (strncmp(statusStart, "在线", 6) == 0) {
                       strcpy(onoff, "online");
                   } else if (strncmp(statusStart, "不在线", 12) == 0) {
                       strcpy(onoff, "offline");
                   }
                this->addFriend(name,onoff);
               }

                qDebug() << "Name:" <<name;
                qDebug() << "Status:" <<onoff;
        }
        else{
            this->updateSharedList(dataString);
            this->updateSharedList(dataString);
            QLabel *popupLabel = new QLabel(this);
                popupLabel->setText( dataString);
                popupLabel->setStyleSheet(
                    "QLabel {"
                    "    background-color: qlineargradient("
                    "        x1: 0, y1: 0, x2: 0, y2: 1, "
                    "        stop: 0 rgba(180, 220, 255, 0.8), "
                    "        stop: 1 rgba(100, 150, 255, 0.8)   "
                    ");"
                    "    border-radius: 5px;"
                    "    padding: 10px;"
                    "    font-size: 14px;"
                    "}");
                popupLabel->setAlignment(Qt::AlignCenter);
                popupLabel->setFixedSize(200, 50);
                popupLabel->move((width() - popupLabel->width()) / 2, 50);
                popupLabel->setWindowFlags(Qt::ToolTip); // 确保消息始终在最前端
                popupLabel->show();
                QTimer::singleShot(2000, popupLabel, &QLabel::deleteLater);
        }
      }
    else if (data.canConvert<unsigned int>()) {
        // 如果数据可以转换为 unsigned int
        unsigned int dataUInt = data.toUInt();
        qDebug() << "Received unsigned int value:" << dataUInt;
    }
    else {
        // 处理其他数据类型
        qDebug() << "Other data type received";
        QMessageBox::information(this, "反馈", "未知数据类型");
    }
}


void logged::updateFriendList() {
    ui->listWidget->clear(); // 清空占位项
    for (int i = 0; i < friendCount; i++) {
        friendItem *friItem = new friendItem();
        QListWidgetItem* m_Item = new QListWidgetItem(ui->listWidget);
        m_Item->setSizeHint(QSize(300, 90));

        connect(&MessageDispatcher::instance(), &MessageDispatcher::messageReceived, friItem, &friendItem::handleResponse);
        //连接消息分发器,每个列表都收消息，但不是自己的消息即跳过，不处理

        ui->listWidget->setItemWidget(m_Item, friItem);
        char name[64] = {0}; // 初始化数组，所有元素设置为 0（即空字符串）
        // 拼接 name
        strcat(name, friendList[i].name);

        // 如果 remark 不为空，拼接 remark
        if (strlen(friendList[i].remark) > 0) {
            strcat(name, "(");
            strcat(name, friendList[i].remark);
            strcat(name, ")");
        }
        char onoff[40];
        snprintf(onoff,sizeof(onoff),"(%s)",friendList[i].status);
        QString message="暂时没有消息...";
        friItem->setFriendInfo(name, onoff,message);
        connect(friItem, &friendItem::clicked, this, [this, friItem]() {

//          FriendChat *chatWindow = new FriendChat(this);
            FriendChat *chatWindow = new FriendChat(friItem, friItem);
            chatWindow->setWindowTitle(friItem->getName() +friItem->getOnOff()); // 设置窗口标题为好友名称
            chatWindow->show();
            connect(friItem,&friendItem::MsgRecvd,chatWindow,&FriendChat::MsgRcvd);
            connect(chatWindow, &FriendChat::requestToSend, sendThread, &SendThread::sendRequest);//设置发送消息的信号绑定到发送线程的函数d
        });
    }

    for(int i=0;i<groupsCount;i++){
        friendItem *friItem = new friendItem();
        QListWidgetItem* m_Item = new QListWidgetItem(ui->listWidget);
        m_Item->setSizeHint(QSize(300, 90));
        ui->listWidget->setItemWidget(m_Item, friItem);
         QString message="暂时没有消息...";
        friItem->setFriendInfo(groups[i].name, QString::number(groups[i].memberCount),message);
    }
}

void logged::on_addfriend_clicked()
{
    AddFriendDialog dialog(this);  // 创建对话框
    dialog.exec();
}

void logged::on_friendReq_clicked()
{
    // 尝试查找已存在的FriendReqList窗口
    FriendReqList *existingFriendList = findChild<FriendReqList *>();

    // 如果没有找到，创建并显示新窗口
    if (!existingFriendList) {
        FriendReqList *friendlist = new FriendReqList(this);
        friendlist->show();
    } else {
        // 如果已经存在该窗口，可以选择将其显示
        existingFriendList->show();
    }
}

void logged::updateSharedList(const QString &newValue)
{
    // 获取当前列表
    sharedList = this->property("sharedList").value<QStringList>();
    sharedList.append(newValue);  // 添加新元素

    // 更新属性
    this->setProperty("sharedList", QVariant::fromValue(sharedList));
}

void logged::removeSharedItem(const QString &valueToRemove)
{
    // 获取当前列表
    sharedList = this->property("sharedList").value<QStringList>();

    // 删除指定的字符串
    sharedList.removeAll(valueToRemove);

    // 更新属性
    this->setProperty("sharedList", QVariant::fromValue(sharedList));
}

void logged::printSharedList()
{
    // 获取当前列表
    sharedList = this->property("sharedList").value<QStringList>();

    // 打印所有的字符串
    qDebug() << "Current List:";
    for (const QString &str : sharedList) {
        qDebug() << str;
    }
}

QStringList logged::getSharedList()
{
    // 获取当前列表
    sharedList = this->property("sharedList").value<QStringList>();

    // 返回列表
    return sharedList;
}


void logged::removeFriend(const char* name) {
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem* item = ui->listWidget->item(i);
        friendItem* friItem = dynamic_cast<friendItem*>(ui->listWidget->itemWidget(item));

        if (friItem && strcmp(friItem->getName().toStdString().c_str(), name) == 0) {
            // 如果找到该好友，删除对应项
            delete friItem;
            delete ui->listWidget->takeItem(i);  // 同时移除 QListWidgetItem
            return;
        }
    }
    qDebug() << "未找到要删除的好友：" << name;
}

void logged::addFriend(const char* name, const char* status) {
    // 创建新的好友项
    friendItem* newFriend = new friendItem();
    QListWidgetItem* newItem = new QListWidgetItem(ui->listWidget);
    newItem->setSizeHint(QSize(300, 90));
    ui->listWidget->setItemWidget(newItem, newFriend);
    connect(&MessageDispatcher::instance(), &MessageDispatcher::messageReceived, newFriend, &friendItem::handleResponse);
    // 设置好友状态
    QString newStatus = QString("(%1)").arg(status);
    connect(newFriend, &friendItem::clicked, this, [this, newFriend]() {

//        FriendChat *chatWindow = new FriendChat(this);
        FriendChat *chatWindow = new FriendChat(newFriend, newFriend);
        chatWindow->setWindowTitle(newFriend->getName() +newFriend->getOnOff()); // 设置窗口标题为好友名称
        chatWindow->show();
        connect(newFriend,&friendItem::MsgRecvd,chatWindow,&FriendChat::MsgRcvd);
        connect(chatWindow, &FriendChat::requestToSend, sendThread, &SendThread::sendRequest);//设置发送消息的信号绑定到发送线程的函数d
    });
    // 设置好友信息
    newFriend->setFriendInfo(name, newStatus, "新增的好友消息...");
}
