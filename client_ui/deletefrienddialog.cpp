#include "deletefrienddialog.h"

DeleteFriendDialog::DeleteFriendDialog(logged *loggedWindow, QWidget *parent)
    : QDialog(parent),
      friendNameInput(new QLineEdit(this)),
      deleteButton(new QPushButton("删除", this)),
      loggedwindow(loggedWindow) // 初始化 loggedwindow
{
    setWindowTitle("删除好友");
    // 布局
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *instructionLabel = new QLabel("请输入要删除的好友名称：", this);
    layout->addWidget(instructionLabel);
    layout->addWidget(friendNameInput);
    layout->addWidget(deleteButton);

    // 删除按钮点击事件
    connect(deleteButton, &QPushButton::clicked, this, &DeleteFriendDialog::onDeleteButtonClicked);
    connect(this, &DeleteFriendDialog::requestToSend, sendThread, &SendThread::sendRequest);

}

DeleteFriendDialog::~DeleteFriendDialog() {}

void DeleteFriendDialog::onDeleteButtonClicked()
{
    QString username = friendNameInput->text().trimmed();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "警告", "好友名称不能为空！");
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
    int action=0;
    request->action = htonl(action);
    strncpy(request->session_token, session_token, sizeof(session_token) - 1);
    request->session_token[sizeof(session_token) - 1] = '\0';


    strncpy(request->friend_username, username.toStdString().c_str(), sizeof(request->friend_username) - 1);
    request->friend_username[sizeof(request->friend_username) - 1] = '\0';  // 确保字符串以 null 结尾
    emit requestToSend(request);
    loggedwindow->removeFriend(username.toStdString().c_str());
}



