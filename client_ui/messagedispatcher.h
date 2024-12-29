#ifndef MESSAGE_DISPATCHER_H
#define MESSAGE_DISPATCHER_H

#include <QObject>
#include <QVariant>

class MessageDispatcher : public QObject {
    Q_OBJECT
public:
    // 获取单例实例
    static MessageDispatcher& instance() {
        static MessageDispatcher dispatcher; // 静态实例，程序生命周期中唯一
        return dispatcher;
    }

    // 分发消息
    void dispatchMessage(const QVariant& data) {
        emit messageReceived(data); // 发射信号
    }

signals:
    // 消息接收信号，供其他对象连接
    void messageReceived(const QVariant& data);

private:
    // 私有构造函数，确保无法直接实例化
    MessageDispatcher(QObject* parent = nullptr) : QObject(parent) {}

    // 禁止拷贝和赋值操作
    ~MessageDispatcher() = default;
    Q_DISABLE_COPY(MessageDispatcher)
};

#endif // MESSAGE_DISPATCHER_H
