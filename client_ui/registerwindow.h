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
signals:
    void requestToSend(void *req); // 定义发送请求的信号
private:
    Ui::registerwindow *ui;
};

#endif // REGISTERWINDOW_H
