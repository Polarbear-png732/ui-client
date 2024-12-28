#ifndef RESPONSETHREAD_H
#define RESPONSETHREAD_H

#include <QThread>
#include <QObject>
#include <QString>

extern "C" {
    void *receive_response(void *arg); // 声明 C 函数
}

// 消息处理类
class ResponseHandler : public QObject {
    Q_OBJECT
public:
    explicit ResponseHandler(QObject *parent = nullptr) : QObject(parent) {}

signals:
    // 信号：将消息传递到主线程
    void messageReceived(const QString &message);

public:
    // 调用此方法以发射信号
    void emitMessage(const QString &message) {
        emit messageReceived(message);
    }
};

class ResponseThread : public QThread
{

    Q_OBJECT

public:
    explicit ResponseThread(QObject *parent = nullptr);
    ~ResponseThread();

protected:
    void run() override;

signals:
    void responseReceived(const QString &message); // 定义信号以通知主线程
private:
    ResponseHandler responseHandler;  // 消息处理类实例
};




#endif // RESPONSETHREAD_H
