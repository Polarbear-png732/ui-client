#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


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

    // 简单的用户名密码校验
    if (username == "admin" && password == "1234") {
        QMessageBox::information(this, "Login Success", "Welcome!");
    } else {
        QMessageBox::warning(this, "Login Failed", "Incorrect username or password.");
    }
}

