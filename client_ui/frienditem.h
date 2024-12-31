#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
namespace Ui {
class friendItem;
}

class friendItem : public QWidget
{
    Q_OBJECT

public:
    explicit friendItem(QWidget *parent = 0);
    ~friendItem();
    void setHeadimage(QString imgpath);
    void setFriendInfo(const QString &name, const QString &status);
    QString getName() const; // 添加获取名称的方法
    QString getOnOff() const; // 添加获取名称的方法
    void handleResponse(const QVariant &data);
protected:
    void mousePressEvent(QMouseEvent *event) override; // 重写鼠标点击事件
signals:
    void clicked(); // 定义点击信号
private:
    Ui::friendItem *ui;
    QLabel *nameLabel;
    QLabel *statusLabel;
    QString message;
};

#endif // FRIENDITEM_H
