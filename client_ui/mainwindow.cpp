#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
extern "C" {
    #include "client.h" // 这是你C语言逻辑代码的头文件
}

LoginRequest *build_login_request(const QString &username, const QString &password)
{
    LoginRequest *request = (LoginRequest *)malloc(sizeof(LoginRequest));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_LOGIN);

    // 将 QString 转换为 C 字符串并复制到结构体中
    strncpy(request->username, username.toStdString().c_str(), sizeof(request->username) - 1);
    request->username[sizeof(request->username) - 1] = '\0';  // 确保字符串以 null 结尾

    strncpy(request->password, password.toStdString().c_str(), sizeof(request->password) - 1);
    request->password[sizeof(request->password) - 1] = '\0';  // 确保字符串以 null 结尾

    request->length = htonl(sizeof(LoginRequest));

    // 发送请求
    send(client_fd, request, sizeof(LoginRequest), 0);

    return request;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 登录按钮点击事件
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoginClicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    build_login_request(username, password);
    // 简单的用户名密码校验
//    if (username == "admin" && password == "1234") {
//        QMessageBox::information(this, "Login Success", "Welcome!");
//    } else {
//        QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
//    }
}

