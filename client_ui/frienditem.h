#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include "ui_frienditem.h"
namespace Ui {
class friendItem;
}

class friendItem : public QWidget
{
    Q_OBJECT

public:
    explicit friendItem(QWidget *parent = nullptr);  // 声明构造函数
    ~friendItem();
    void setHeadimage(QString imgpath);
    void updateMsg(const QString &msg);  // 添加 updateMsg 函数的声明
    void updateOnoff(const QString &onoff);
    void setFriendInfo(const QString &name, const QString &status,const QString &message);
    QString getName() const; // 添加获取名称的方法
    QString getOnOff() const; // 添加获取名称的方法
    void handleResponse(const QVariant &data);
    void addReceivedMessage(const QString &message); // 添加接收消息
    void addSentMessage(const QString &message);     // 添加发送消息
    // 获取消息记录
    QStringList getMessageHistory() const;
protected:
    void mousePressEvent(QMouseEvent *event) override; // 重写鼠标点击事件
signals:
    void clicked(); // 定义点击信号
    void MsgRecvd(const QString msg);
private:
    Ui::friendItem *ui;
    QLabel *nameLabel;
    QLabel *statusLabel;
    QLabel *msgLabel;
    QString message;
    QStringList messageHistory;  // 消息记录
};

#endif // FRIENDITEM_H
