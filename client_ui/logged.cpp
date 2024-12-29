#include "logged.h"
#include "ui_logged.h"
#include <QListWidget>
#include <QCloseEvent>
#include "frienditem.h"
#include "responsethread.h"
#include "sendthread.h"
#include <QDebug>
#include <QMessageBox>
extern ResponseThread* responseThread;
extern SendThread* sendThread;

logged::logged(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::logged)
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
        // 如果数据是 QString 类型
        QString dataString = data.toString();
        qDebug() << "String message:" << dataString;

        QMessageBox::information(this, "反馈", dataString);
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
}
