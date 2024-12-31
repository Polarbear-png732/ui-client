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
    explicit logged(QWidget *parent = nullptr);
    ~logged();
    void handleResponse(const QVariant &data);
    void updateSharedList(const QString &newValue);
    void removeSharedItem(const QString &valueToRemove);
    void printSharedList();
    QStringList getSharedList();
protected:
    void closeEvent(QCloseEvent *event) override; // 处理关闭事件
signals:
    void requestToSend(void *req); // 定义发送请求的信号
private slots:
    void updateFriendList();
    void on_addfriend_clicked();

    void on_friendReq_clicked();

private:
    Ui::logged *ui;
    QStringList sharedList;//用于存储好友请求列表

};

#endif // LOGGED_H
