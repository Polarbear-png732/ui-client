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
    void messageReceived(const QVariant &data);

public:
    void emitMessage(const QVariant &data) {
        emit messageReceived(data);
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
    void responseReceived(const QVariant &data); // 定义信号以通知主线程
private:
    ResponseHandler responseHandler;  // 消息处理类实例
};




#endif // RESPONSETHREAD_H
