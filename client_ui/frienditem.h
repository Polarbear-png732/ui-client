#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>

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
private:
    Ui::friendItem *ui;
};

#endif // FRIENDITEM_H
