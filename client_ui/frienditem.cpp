#include "frienditem.h"
#include "ui_frienditem.h"

friendItem::friendItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::friendItem)
{
    ui->setupUi(this);
     this->setAttribute(Qt::WA_Hover); // 启用鼠标追踪
}

friendItem::~friendItem()
{
    delete ui;
}

void friendItem::setHeadimage(QString imgpath){
    // 构建样式表字符串，确保图片路径是URL格式
    QString styleSheet = QString("border-image: url(%1);").arg(imgpath);
    ui->label->setStyleSheet(styleSheet);
}


