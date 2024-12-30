#include "logged.h"
#include "ui_logged.h"
#include <QListWidget>
#include <QCloseEvent>
#include "frienditem.h"
#include "responsethread.h"
#include "sendthread.h"
#include <QDebug>
#include <QMessageBox>

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
    for (int i = 0; i < 5; i++) { // 假设显示空的占位项
             friendItem *friItem = new friendItem();
             QListWidgetItem* m_Item = new QListWidgetItem(ui->listWidget);
             m_Item->setSizeHint(QSize(300, 90));
             ui->listWidget->setItemWidget(m_Item, friItem);
        }

        ui->listWidget->setStyleSheet("QListWidget::item:hover {"
                                      "background-color: rgb(200,200,200);"
                                      "}");

//    pthread_mutex_lock(&loggedui_mutex);
//    pthread_cond_wait(&loggedui_cond, &loggedui_mutex);  //等待接收完消息，再开始渲染

//    for(int i=0;i<friendCount;i++){
//         friendItem * friItem=new friendItem();
//         QListWidgetItem* m_Item=new QListWidgetItem(ui->listWidget);
//         m_Item->setSizeHint(QSize(300,90));
//         ui->listWidget->setItemWidget(m_Item,friItem);
//         friItem->setFriendInfo(friendList[i].name,friendList[i].status);
//     }
//     pthread_mutex_lock(&loggedui_mutex);
//     ui->listWidget->setStyleSheet("QListWidget::item:hover {"
//                                   "background-color: rgb(200,200,200);"
//                                   "}");
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
    qDebug() << "Received data type:" << data.typeName();

    // 打印原始数据
    qDebug() << "Raw data:" << data;

    if (data.type() == QVariant::String) {
        qDebug() << "QString type:" << data;
        // 如果数据是 QString 类型
        QString dataString = data.toString();
        char* message = dataString.toUtf8().data(); // 或者 dataString.toStdString().c_str();
        if (strncmp(message, "好友", 6) == 0) { // "好友" 是 6 个字节
            parseFriendList(message);
            QMetaObject::invokeMethod(this, "updateFriendList", Qt::QueuedConnection);
           } else{
            parseGroupInfo(message,groups,&groupsCount);
            for (int i = 0; i < groupsCount;i++) {
                qDebug() << "群聊名称:" << groups[i].name;
                qDebug() << "群聊ID:" << groups[i].id;
                qDebug() << "群主:" << groups[i].owner;
                qDebug() << "成员总数:" << groups[i].memberCount;
                qDebug() << "成员列表:";
                for (int j = 0; j < groups[i].memberCount; j++) {
                    qDebug() << "  -" << groups[i].members[j];
                }
                qDebug() << "";
            }
        }
      }

    else if (data.canConvert<unsigned int>()) {
        // 如果数据可以转换为 unsigned int
        unsigned int dataUInt = data.toUInt();
        qDebug() << "Received unsigned int value:" << dataUInt;

        // 显示消息框
        QMessageBox::information(this, "反馈", QString::number(dataUInt));
    }
    else {
        // 处理其他数据类型
        qDebug() << "Other data type received";
        QMessageBox::information(this, "反馈", "未知数据类型");
    }
    pthread_cond_signal(&loggedui_cond);
}


void logged::updateFriendList() {
    ui->listWidget->clear(); // 清空占位项
    for (int i = 0; i < friendCount; i++) {
        friendItem *friItem = new friendItem();
        QListWidgetItem* m_Item = new QListWidgetItem(ui->listWidget);
        m_Item->setSizeHint(QSize(300, 90));
        ui->listWidget->setItemWidget(m_Item, friItem);
        friItem->setFriendInfo(friendList[i].name, friendList[i].status);
    }
}
