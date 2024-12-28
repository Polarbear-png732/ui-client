#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "registerwindow.h"
#include <QTimer>
#include <QDebug>
#include <QString>
extern "C" {
    #include "client.h" // 这是你C语言逻辑代码的头文件
}
void *receive_response(void *arg);
void *build_login_request(const QString &username, const QString &password);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    responseThread(nullptr)  // 初始化为nullptr
{
    ui->setupUi(this);
    statusBar()->showMessage("Ready");
    // 连接按钮的槽函数
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    QTimer::singleShot(0, this, &MainWindow::startResponseThread);
}


MainWindow::~MainWindow()
{
    if (responseThread->isRunning()) {
        responseThread->requestInterruption(); // 请求中断
        responseThread->quit();               // 退出线程事件循环
        responseThread->wait();               // 等待线程退出
    }
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

void MainWindow::onRegisterClicked()
{
    this->hide(); // 隐藏登录窗口
    QTimer::singleShot(0, this, &MainWindow::startResponseThread);
    registerwindow *regWindow = new registerwindow(this); // 设置父窗口为 MainWindow
    regWindow->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
    regWindow->show(); // 显示注册窗口
}

void MainWindow::startResponseThread()
{
    // 在这里启动响应线程
    responseThread = new ResponseThread(this);

    // 将线程信号与槽连接
    connect(responseThread, &ResponseThread::responseReceived, this, &MainWindow::handleResponse);

    // 线程结束时自动删除线程对象
    connect(responseThread, &ResponseThread::finished, responseThread, &QObject::deleteLater);

    responseThread->start();
    qDebug() << "Response thread started!";
}
void MainWindow::handleResponse(const QVariant &data)
{

    if (data.canConvert<QString>()) {
        qDebug() << "String message:" << data.toString();
    } else if (data.canConvert<int>()) {
        qDebug() << "Integer message:" << data.toInt();
    } else {
        qDebug() << "Other data type received";
    }
    QMessageBox::information(this, "反馈", "未知数据类型");

//    ui->statusBar->showMessage(message, 5000);
}



void *build_login_request(const QString &username, const QString &password)
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
    free(request);

}
void *receive_response(void *arg)
{
    ResponseHandler *handler = (ResponseHandler *)arg;
    char buffer[BUFSIZE];
    unsigned int req_length;
    unsigned int size_len = sizeof(req_length);
    while (1)
    {
        if (recv_full(client_fd, buffer, size_len) == 0)
        {
            break;
        } // 先接收报文长度
        req_length = ntohl(*(unsigned int *)buffer);                    // 将接收到的报文长度从网络字节序转换为主机字节序
        recv_full(client_fd, buffer + size_len, req_length - size_len); // 接收剩余的数据
        unsigned int response_code = ntohl(*(unsigned int *)(buffer + size_len));

        switch (response_code)
        {
        case RESPONSE_LOGIN:
        {
            LoginResponse *response = (LoginResponse *)buffer;
            printf("Login response:\n");
            printf("Status Code: %u\n", ntohl(response->status_code));
            strncpy(session_token, response->session_token, sizeof(session_token) - 1);
            session_token[sizeof(session_token) - 1] = '\0';
            printf("%s\n", session_token);
            QString message = QString::fromUtf8(response->session_token);
            // 发射信号，将消息传递到主线程
            handler->emitMessage(message);
            break;
        }
        case SIMPLE_RESPONSE:
        {
            SimpleResponse *resp = (SimpleResponse *)buffer;
            int status=ntohl(resp->status_code);
            QString message;
                    if (status == 200) {
                        message = "操作成功";
                    } else {
                        message = "操作失败";
                    }
             handler->emitMessage(message);

            break;
        }
        case RESPONSE_FILE_ACK:
        {
            printf("第%u块，确认开始传输\n", *(unsigned *)(buffer + 8)); // 检查是否进入该 case
            file_transfer(buffer);
            break;
        }
        case REQUEST_FILE_TRANSFER:
        {
            printf("开始接收文件！ \n");
            int file_sock = file_sock_init(); // 收到传输请求再调用函数创建套接字，保证服务器accept到正确的套接字上
            file_recv(buffer, file_sock);
            close(file_sock);
            break;
        }
        default:
        {
            FeedbackMessage *message = (FeedbackMessage *)buffer;

            QString message_1 = QString::fromUtf8(message->message);
            // 发射信号，将消息传递到主线程
            handler->emitMessage(message_1);
            break;
        }
        }
    }
    return NULL;
}

