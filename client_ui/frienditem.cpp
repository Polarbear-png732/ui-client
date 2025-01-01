#include "frienditem.h"
#include "messagedispatcher.h"
#include <QDebug>
#include "ui_frienditem.h"

friendItem::friendItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::friendItem)  // 确保正确初始化ui
{
    ui->setupUi(this); // 设置UI

    // 启用鼠标追踪
    this->setAttribute(Qt::WA_Hover);

    // 直接访问UI控件
    nameLabel = ui->nameLabel; // 直接从UI对象获取控件
    statusLabel = ui->onoffLabel;
    msgLabel=ui->msgLabel;


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

void friendItem::setFriendInfo(const QString &name,  const QString &status,const QString &msg)
{
    nameLabel->setText(name);
    statusLabel->setText(status);
    msgLabel->setText(msg);
}
void friendItem::updateMsg(const QString &msg)
{
    msgLabel->setText(msg);
}

void friendItem::updateOnoff(const QString &onoff)
{
    statusLabel->setText(onoff);
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
    QString name = nameLabel->text(); // 获取 nameLabel 的文本

    // 使用正则表达式匹配名字和备注
    QRegularExpression regex("^(.*?)(?:\\(.*?\\))?$");
    QRegularExpressionMatch match = regex.match(name);

    if (match.hasMatch()) {
        // 提取名字部分（括号之前的内容）
        return match.captured(1).trimmed(); // 去除可能的空白字符
    }

    // 如果没有匹配到，直接返回原始字符串（理论上不会走到这里）
    return name.trimmed();
}
QString friendItem::getOnOff() const
{
    return statusLabel->text();
}

void friendItem::handleResponse(const QVariant &data)
{
    qDebug() << "friendItem::handleResponse row data" << data;
    QString dataString = data.toString();
    qDebug() << dataString;
    QString friendName=this->getName();
    qDebug() << friendName;
    QString expectedPrefix = "私聊消息" + friendName + ":";
    qDebug() << expectedPrefix;

    QString message;
    if (data.type() == QVariant::String) {
        qDebug() << "99999999999999999";
         // Convert and save the string
        if (dataString.startsWith(expectedPrefix)) {
            message = dataString.mid(expectedPrefix.length());
            qDebug() << message;
            this->updateMsg(message);//更新消息u
            emit MsgRecvd(message);
            qDebug() << "emit MsgRecvd";
        }

    }

}




