#ifndef LOGGED_H
#define LOGGED_H

#include <QMainWindow>

namespace Ui {
class logged;
}

class logged : public QMainWindow
{
    Q_OBJECT

public:
    explicit logged(QWidget *parent = 0);
    ~logged();

private:
    Ui::logged *ui;
};

#endif // LOGGED_H
