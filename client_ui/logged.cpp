#include "logged.h"
#include "ui_logged.h"
#include <QListWidget>
#include <QCloseEvent>
#include "frienditem.h"
#include "responsethread.h"
#include "sendthread.h"
extern ResponseThread* responseThread;
extern SendThread* sendThread;

logged::logged(QWidget *parent, QMainWindow *loginWindow) :
    QMainWindow(parent),
    ui(new Ui::logged),
    loginWindow(loginWindow) // 保存登录窗口指针
{
    ui->setupUi(this);
    for(int i=1;i<20;i++){
         friendItem * friItem=new friendItem();

         QListWidgetItem* m_Item=new QListWidgetItem(ui->listWidget);
         m_Item->setSizeHint(QSize(300,90));
         ui->listWidget->setItemWidget(m_Item,friItem);

     }
     ui->listWidget->setStyleSheet("QListWidget::item:hover {"
                                   "background-color: rgb(200,200,200);"  // 设置悬停时的背景色为黄色
                                   "}");
}

logged::~logged()
{
    delete ui;
}

// 在关闭 logged 窗口时重新显示登录窗口
void logged::closeEvent(QCloseEvent *event)
{
    if (loginWindow) {
        loginWindow->show();  // 显示登录窗口
    }
    event->accept();  // 允许关闭
}
