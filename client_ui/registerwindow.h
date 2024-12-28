#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QMainWindow>



namespace Ui {
    class registerwindow;
}

class registerwindow : public QMainWindow  // 改为继承 QMainWindow
{
    Q_OBJECT

public:
    explicit registerwindow(QWidget *parent = nullptr);
    ~registerwindow();
protected:
    void closeEvent(QCloseEvent *event) override;
private slots:
    void register_2_clicked();

private:
    Ui::registerwindow *ui;
};

#endif // REGISTERWINDOW_H
