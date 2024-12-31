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
//    qDebug() << "Received data type:" << data.typeName();

//    // 打印原始数据
//    qDebug() << "Raw data:" << data;

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
                // 设置大小和位置
                popupLabel->setFixedSize(200, 50);
                popupLabel->move((width() - popupLabel->width()) / 2, 50);
                popupLabel->setWindowFlags(Qt::ToolTip); // 确保消息始终在最前端
                popupLabel->show();
                QTimer::singleShot(2000, popupLabel, &QLabel::deleteLater);
                this->printSharedList();
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
    for(int i=0;i<groupsCount;i++){
        friendItem *friItem = new friendItem();
        QListWidgetItem* m_Item = new QListWidgetItem(ui->listWidget);
        m_Item->setSizeHint(QSize(300, 90));
        ui->listWidget->setItemWidget(m_Item, friItem);
        friItem->setFriendInfo(groups[i].name, QString::number(groups[i].memberCount));
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
