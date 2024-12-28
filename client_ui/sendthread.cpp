#include "sendthread.h"
#include <QMutexLocker>
#include <unistd.h> // For usleep if needed
#include <cstdlib>  // For malloc and free
#include <cstdio>   // For perror

SendThread::SendThread(QObject *parent)
    : QThread(parent), stopFlag(false) // 初始化停止标志
{
}

SendThread::~SendThread()
{
    stop(); // 确保线程停止
    wait(); // 等待线程完全退出
}

void SendThread::stop()
{
    QMutexLocker locker(&mutex);
    stopFlag = true;        // 设置停止标志
    condition.wakeAll();    // 唤醒线程以便退出
}

void SendThread::sendRequest(void *req)
{
    QMutexLocker locker(&mutex);
    if (req) {
        pendingRequests.enqueue(req); // 将请求加入队列
        condition.wakeOne();         // 唤醒线程处理
    }
}

void SendThread::run()
{
    while (true) {
        void *request = nullptr;

        {
            QMutexLocker locker(&mutex);

            // 等待队列中有请求或线程被停止
            while (pendingRequests.isEmpty() && !stopFlag) {
                condition.wait(&mutex); // 等待条件变量
            }

            // 如果停止标志已设置且队列为空，则退出线程
            if (stopFlag && pendingRequests.isEmpty()) {
                break;
            }

            // 从队列中取出请求
            if (!pendingRequests.isEmpty()) {
                request = pendingRequests.dequeue();
            }
        }

        // 发送请求
        if (request) {
            unsigned int length = *(unsigned int *)request; // 假设请求前 4 字节是长度
            int result = send(client_fd, request, length, 0); // 发送请求
            free(request); // 释放动态分配的内存

            if (result == -1) {
                perror("Send failed"); // 打印错误信息
            }
        }
    }
}
