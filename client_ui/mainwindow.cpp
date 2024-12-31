#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "registerwindow.h"
#include <QTimer>
#include <QDebug>
#include <QString>
#include "sendthread.h"
#include "logged.h"
#include "messagedispatcher.h"
extern "C" {
    #include "client.h" // 这是你C语言逻辑代码的头文件
}
extern int client_fd;
extern ResponseThread* responseThread;
extern SendThread* sendThread;

void *receive_response();
void *build_login_request(const QString &username, const QString &password);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 连接按钮的槽函数
    connect(ui->loginButton, &QPushButton::clicked, this, &MainWindow::onLoginClicked);
    connect(ui->registerButton, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);

    connect(this, &MainWindow::requestToSend_1, sendThread, &SendThread::sendRequest);  //连接发送线程

    connect(&MessageDispatcher::instance(), &MessageDispatcher::messageReceived, this, &MainWindow::handleResponse);

}


MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::onLoginClicked()
{
    qDebug() << "onLoginclicked:";
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

    emit requestToSend_1(request);
    qDebug() << "Signal emitted, requestLogin sent.";
}

void MainWindow::onRegisterClicked()
{
    this->hide(); // 隐藏登录窗口
    registerwindow *regWindow = new registerwindow(this); // 设置父窗口为 MainWindow

    connect(regWindow, &registerwindow::requestToSend, sendThread, &SendThread::sendRequest);//设置发送消息的信号绑定到发送线程的函数
    connect(&MessageDispatcher::instance(), &MessageDispatcher::messageReceived, regWindow, &registerwindow::handleResponse);

    regWindow->setAttribute(Qt::WA_DeleteOnClose); // 窗口关闭时自动删除
    regWindow->show(); // 显示注册窗口
}
void MainWindow::handleResponse(const QVariant &data)
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
            if (dataUInt == 10021) {
                disconnect(&MessageDispatcher::instance(), &MessageDispatcher::messageReceived, this, &MainWindow::handleResponse);
                qDebug() << "Logged started!";
                this->hide();
                logged *friendlist = new logged(this);                    //登录后的窗口
                friendlist->setAttribute(Qt::WA_DeleteOnClose);
                friendlist->show();
                connect(friendlist, &logged::requestToSend, sendThread, &SendThread::sendRequest);
                connect(&MessageDispatcher::instance(), &MessageDispatcher::messageReceived, friendlist, &logged::handleResponse);
                pthread_cond_signal(&cond);      //窗口切换成功，发送激发条件变量，让接收进程发送相关多消息过来
            return;
            }

            message = QString::number(dataUInt);
        }

    QMessageBox::information(this, "反馈", message);
    // ui->statusBar->showMessage(message, 5000);
}



void *receive_response()
{
    char buffer[BUFSIZE];
    unsigned int req_length;
    unsigned int size_len = sizeof(req_length);
    int i=0;
    while (1)
    {
        qDebug() << "Client FD:" << QString::number(client_fd);
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
            MessageDispatcher::instance().dispatchMessage(req_code);

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
             MessageDispatcher::instance().dispatchMessage(message);

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
            qDebug() << "message:" << message_1;
            pthread_mutex_lock(&mutex);
            if(i==0){
            pthread_cond_wait(&cond, &mutex);
            i++;
            }
            MessageDispatcher::instance().dispatchMessage("flaskhgkas");
            MessageDispatcher::instance().dispatchMessage(message_1);
            pthread_mutex_unlock(&mutex);
            break;
        }
        }
    }
    return NULL;
}

