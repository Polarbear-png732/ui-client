#ifndef FRIENDREQLIST_H
#define FRIENDREQLIST_H
#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "logged.h"
class FriendReqList : public QMainWindow
{
    Q_OBJECT

public:
    explicit FriendReqList(logged *parent = nullptr);
    ~FriendReqList();
    void updateUI();
protected:
    void closeEvent(QCloseEvent *event) override;
    void onItemClicked(QListWidgetItem *item);  // 槽函数，用于处理点击事件
private:
    QListWidget *friendRequestList;  // 用于显示好友请求的列表
    logged * q;
    void setupUI();  // 设置用户界面
};


#endif // FRIENDREQLIST_H
