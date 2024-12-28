#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "registerwindow.h"
#include <QTimer>
#include <QDebug>
#include <QString>
#include "sendthread.h"
#include "logged.h"

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
    if (sendThread) {
        sendThread->stop(); // 自定义的停止方法，设置标志并唤醒线程
        sendThread->wait(); // 等待线程安全退出
        delete sendThread;
    }
    delete ui;
}

void MainWindow::onLoginClicked()
{
    QString username = ui->usernameLineEdit->text();
    QString password = ui->passwordLineEdit->text();

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
    emit requestToSend(request);
}

void MainWindow::onRegisterClicked()
{
    this->hide(); // 隐藏登录窗口
    QTimer::singleShot(0, this, &MainWindow::startResponseThread);
    registerwindow *regWindow = new registerwindow(this); // 设置父窗口为 MainWindow
    connect(regWindow, &registerwindow::requestToSend, sendThread, &SendThread::sendRequest);//将请求发到发送线程进行发送
    regWindow->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
    regWindow->show(); // 显示注册窗口
}

void MainWindow::startResponseThread()
{


    sendThread = new SendThread(this); // 初始化发送线程
    connect(sendThread, &SendThread::finished, sendThread, &QObject::deleteLater);
    sendThread->start();
    connect(this, &MainWindow::requestToSend, sendThread, &SendThread::sendRequest);

    responseThread = new ResponseThread(this);
    connect(responseThread, &ResponseThread::responseReceived, this, &MainWindow::handleResponse);
    connect(responseThread, &ResponseThread::finished, responseThread, &QObject::deleteLater);
    responseThread->start();

    qDebug() << "Response thread and Send thread started!";
}


void MainWindow::handleResponse(const QVariant &data)
{
    QString message; // 用于存储将要显示的消息

    if (data.canConvert<QString>()) {
        QString dataString = data.toString(); // 转换并保存字符串
        qDebug() << "String message:" << dataString;
        message = dataString;
    } else if (data.canConvert<unsigned int>()) {
        unsigned int dataUInt = data.toUInt();
        qDebug() << "Integer message:" << dataUInt;  //登录成功，跳转新窗口
        if(dataUInt==10021){
            this->hide(); // 隐藏登录窗口
            logged *friendlist = new logged(this, this);
            friendlist->setAttribute(Qt::WA_DeleteOnClose);
            friendlist->show();
        }
        message = QString::number(dataUInt); // 将整数转换为字符串并赋值给 message
    } else {
        qDebug() << "Other data type received";
        message = "未知数据类型"; // 为其他类型设置消息
    }

    // 显示消息框
    QMessageBox::information(this, "反馈", message);

    // 如果您想要在状态栏中显示消息，请取消注释以下代码
    // ui->statusBar->showMessage(message, 5000);
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
            unsigned int req_code=ntohl(response->request_code);
            printf("Login response:\n");
            printf("Status Code: %u\n", ntohl(response->status_code));
            strncpy(session_token, response->session_token, sizeof(session_token) - 1);
            session_token[sizeof(session_token) - 1] = '\0';
            printf("%s\n", session_token);
            QString message = QString::fromUtf8(response->session_token);

            handler->emitMessage(req_code);
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

