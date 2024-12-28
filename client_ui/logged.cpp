#include "logged.h"
#include "ui_logged.h"
#include "frienditem.h"
#include <QListWidget>
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
