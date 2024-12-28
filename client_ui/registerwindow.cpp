#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "mainwindow.h"
#include <QCloseEvent>
#include "client.h"

registerwindow::registerwindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::registerwindow)
{
    ui->setupUi(this);
      connect(ui->register_2, &QPushButton::clicked, this, &registerwindow::register_2_clicked);
}

registerwindow::~registerwindow()
{
    delete ui;
}

//void registerwindow::onClose()
//{
//    // 当注册窗口关闭时，显示登录窗口
//    MainWindow *mainWindow = new MainWindow();
//    mainWindow->show();
//}

CreateUser *build_create_user_request(const QString &username, const QString &password)
{
    CreateUser* request =(CreateUser*)malloc(sizeof(CreateUser));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_CREATEUSER);
    strncpy(request->username, username.toStdString().c_str(), sizeof(request->username) - 1);
    request->username[sizeof(request->username) - 1] = '\0';  // 确保字符串以 null 结尾

    strncpy(request->password, password.toStdString().c_str(), sizeof(request->password) - 1);
    request->password[sizeof(request->password) - 1] = '\0';  // 确保字符串以 null 结尾

    request->length = htonl(sizeof(CreateUser));
    send(client_fd, request, sizeof(CreateUser), 0);
}

void registerwindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event); // 保留基类行为
    if (parentWidget()) {
        parentWidget()->show(); // 关闭注册窗口时显示父窗口
    }
    // 如果需要执行额外的关闭逻辑，可以在这里添加
}

void registerwindow::register_2_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();
    build_create_user_request(username,password);

}
