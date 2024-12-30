#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
#include <QLabel>
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
private:
    Ui::friendItem *ui;
    QLabel *nameLabel;
    QLabel *statusLabel;
};

#endif // FRIENDITEM_H
