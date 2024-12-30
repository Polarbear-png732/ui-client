#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <sendthread.h>
extern "C" {
    #include "client.h" //C语言逻辑代码的头文件
}
extern SendThread* sendThread;
class AddFriendDialog : public QDialog {
    Q_OBJECT

public:
        explicit AddFriendDialog(QWidget *parent = nullptr);

signals:
    void requestToSend(void *req); // 定义发送请求的信号
private slots:
    void sendFriendRequest(){
        QString username = friendInput->text().trimmed();

        if (username.isEmpty()) {
            QMessageBox::warning(this, "提示", "请输入好友用户名！");
            return;
        }
        FriendRequest *request = (FriendRequest *)malloc(sizeof(FriendRequest));
        if (!request)
        {
            perror("内存分配失败");
            exit(1);
        }
        request->request_code = htonl(REQUEST_ADD_FRIEND);
        request->length = htonl(sizeof(FriendRequest));
        int action=1;
        request->action = htonl(action);
        strncpy(request->session_token, session_token, sizeof(session_token) - 1);
        request->session_token[sizeof(session_token) - 1] = '\0';


        strncpy(request->friend_username, username.toStdString().c_str(), sizeof(request->friend_username) - 1);
        request->friend_username[sizeof(request->friend_username) - 1] = '\0';  // 确保字符串以 null 结尾
        emit requestToSend(request);
        // 关闭对话框
        accept();
    }

private:
    QLineEdit *friendInput;  // 用户名输入框
};
