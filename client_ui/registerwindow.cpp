#include "registerwindow.h"
#include "ui_registerwindow.h"
#include "mainwindow.h"
#include <QCloseEvent>
#include "client.h"
#include "sendthread.h"
#include <QDebug>
#include <QMessageBox>
extern ResponseThread* responseThread;
extern SendThread* sendThread;
extern int client_fd;
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


void registerwindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event); // 保留基类行为
    if (parentWidget()) {
        parentWidget()->show(); // 关闭注册窗口时显示父窗口
    }    
}

void registerwindow::register_2_clicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

    // 构造请求
    CreateUser *request = (CreateUser *)malloc(sizeof(CreateUser));
    if (!request) {
        perror("内存分配失败");
        return;
    }

    request->request_code = htonl(REQUEST_CREATEUSER);
    strncpy(request->username, username.toStdString().c_str(), sizeof(request->username) - 1);
    request->username[sizeof(request->username) - 1] = '\0';
    strncpy(request->password, password.toStdString().c_str(), sizeof(request->password) - 1);
    request->password[sizeof(request->password) - 1] = '\0';
    request->length = htonl(sizeof(CreateUser));

    // 发出信号，将请求传递给发送线程
    emit requestToSend(request);

}

void registerwindow::handleResponse(const QVariant &data)
{
    QString message;
    qDebug() << "Received QVariant data type:" << data.typeName();
        qDebug() << "Raw data:" << data;

    if (data.type() == QVariant::String) {
        QString dataString = data.toString(); // Convert and save the string
        qDebug() << "String message:" << dataString;
        message = dataString;
    }
    else{
            unsigned int dataUInt = data.toUInt();
            qDebug() << "Received unsigned int value:" << dataUInt;
            message = QString::number(dataUInt);
        }
    QMessageBox::information(this, "反馈", message);
    // ui->statusBar->showMessage(message, 5000);
}

