#include "frienditem.h"
#include "ui_frienditem.h"
#include "messagedispatcher.h"
friendItem::friendItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::friendItem)
{
    ui->setupUi(this);
     this->setAttribute(Qt::WA_Hover); // 启用鼠标追踪
    nameLabel = ui->nameLabel;
    statusLabel = ui->onoffLabel;
}

friendItem::~friendItem()
{
    delete ui;
}

void friendItem::setHeadimage(QString imgpath){
    // 构建样式表字符串，确保图片路径是URL格式
    QString styleSheet = QString("border-image: url(%1);").arg(imgpath);
    ui->headimg->setStyleSheet(styleSheet);
}
void friendItem::setFriendInfo(const QString &name,  const QString &status)
{
    nameLabel->setText(name);
    statusLabel->setText(status);
}


void friendItem::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked(); // 发射点击信号
    }
    QWidget::mousePressEvent(event); // 调用基类的事件处理函数
}
QString friendItem::getName() const
{
    return nameLabel->text(); // 返回 nameLabel 的文本
}
QString friendItem::getOnOff() const
{
    return statusLabel->text();
}

void MainWindow::handleResponse(const QVariant &data)
{
    QString message;
    qDebug() << "Received QVariant data type:" << data.typeName();
        qDebug() << "Raw data:" << data;

    if (data.type() == QVariant::String) {
s
        QString dataString = data.toString(); // Convert and save the string
        qDebug() << "String message:" << dataString;
        message = dataString;
    }


    QMessageBox::information(this, "反馈", message);
    // ui->statusBar->showMessage(message, 5000);
}




