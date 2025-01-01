#include "friendreqlist.h"
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QMessageBox>
#include "logged.h"
#include "handleaddfrienddialog.h"
FriendReqList::FriendReqList(logged *parent)
    : QMainWindow(parent)  // 将 logged* 传递给 QMainWindow 的构造函数
{
    q=parent;
    setupUI();
    connect(friendRequestList, &QListWidget::itemClicked, this, &FriendReqList::onItemClicked);
}

FriendReqList::~FriendReqList()
{

}

void FriendReqList::setupUI()
{
    // 设置窗口标题和大小
    setWindowTitle("好友请求");
    setFixedSize(300, 400);

    // 创建 QListWidget 用于显示好友请求列表
    friendRequestList = new QListWidget(this);


    QStringList friendRequests;

    // 添加一些好友请求
    friendRequests=q->getSharedList();
    // 循环遍历好友请求并添加到 QListWidget
    for (const QString &request : friendRequests) {
        QListWidgetItem *item = new QListWidgetItem(request, friendRequestList);
        item->setTextAlignment(Qt::AlignCenter);  // 设置文本居中

        // 设置项的大小
        item->setSizeHint(QSize(200, 50));  // 宽度 200，高度 50

        // 设置字体大小
        QFont font = item->font();  // 获取当前字体
        font.setPointSize(14);      // 设置字体大小为 14
        item->setFont(font);        // 应用新字体
    }


    // 创建垂直布局并添加控件
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(friendRequestList);

    // 创建一个中央窗口控件
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);

    // 设置窗口的中央控件
    setCentralWidget(centralWidget);
}
void FriendReqList::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event); // 保留基类行为
    if (parentWidget()) {
        parentWidget()->show(); // 关闭注册窗口时显示父窗口
    }
}

void FriendReqList::onItemClicked(QListWidgetItem *item)
{
    if (item) {
        // 获取被点击的项的文本
        QString itemText = item->text();
        HandleFriendDialog *dialog = new HandleFriendDialog(itemText, this);
        dialog->exec();  // 或者使用 dialog->show() 非模态显示

    }
}

// 更新 UI 的函数
void FriendReqList::updateUI()
{
    // 清空列表
    friendRequestList->clear();

    // 假设 friendRequests 是一个 QStringList，包含好友请求
    QStringList friendRequests = {"Request 1", "Request 2", "Request 3"};

    // 添加好友请求到 QListWidget
    for (const QString &request : friendRequests) {
        QListWidgetItem *item = new QListWidgetItem(request, friendRequestList);
        item->setTextAlignment(Qt::AlignCenter);  // 设置文本居中
    }
}


