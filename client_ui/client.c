#include "client.h"


int client_fd;                                    // 客户端套接字
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // 互斥锁
char session_token[64];                           // 会话标识符

char file_name[256];


void start_client()
{
    init_client();
    return 0;
}

void exit_client()
{
    unsigned int len = sizeof(ClientExit);
    ClientExit *exit = (ClientExit *)malloc(len);
    exit->length = htonl(len);
    exit->request_code = htonl(CLIENT_EXIT);
    send(client_fd, exit, len, 0);
    free(exit);
    pthread_mutex_lock(&lock);
    close(client_fd); // 确保socket资源释放
    pthread_mutex_unlock(&lock);
    pid_t pid = getpid();
    kill(pid, SIGKILL);
}

CreateUser *build_create_user_request()
{
    CreateUser *request = malloc(sizeof(CreateUser));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_CREATEUSER);
    printf("请输入用户名: ");
    scanf("%s", request->username);
    printf("请输入密码: ");
    scanf("%s", request->password);
    request->length = htonl(sizeof(CreateUser));
    return request;
}

FriendRequest *build_friend_request()
{
    FriendRequest *request = malloc(sizeof(FriendRequest));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_ADD_FRIEND);
    printf("添加好友输入1，删除好友输入0\n");
    int action;
    scanf("%d", &action);
    printf("请输入好友名字：\n");
    scanf("%s", request->friend_username);
    request->action = htonl(action);
    strncpy(request->session_token, session_token, sizeof(session_token) - 1);
    request->session_token[sizeof(session_token) - 1] = '\0';
    request->length = htonl(sizeof(FriendRequest));
    return request;
}

HandleFriendRequest *build_handle_friend_request()
{
    HandleFriendRequest *request = malloc(sizeof(HandleFriendRequest));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_HANDELE_ADD);
    printf("处理哪一个好友请求？请输入对方名称：\n");
    scanf("%s", request->friend_username);
    printf("是否接受好友请求，是输入1，否输入0\n");
    int action;
    scanf("%d", &action);
    request->action = htonl(action);
    strncpy(request->session_token, session_token, sizeof(session_token) - 1);
    request->session_token[sizeof(session_token) - 1] = '\0';
    request->length = htonl(sizeof(HandleFriendRequest));
    return request;
}

PrivateMessage *build_private_message_request()
{
    PrivateMessage *request = malloc(sizeof(PrivateMessage));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_PRIVATE_MESSAGE);
    printf("请输入接收消息的用户：\n");
    scanf("%s", request->receiver_username);
    printf("请输入消息：\n");
    scanf("%s", request->message);
    strncpy(request->session_token, session_token, TOKEN_LEN - 1);
    request->session_token[TOKEN_LEN - 1] = '\0';
    request->length = htonl(sizeof(PrivateMessage));
    return request;
}

GroupCreateRequest *build_group_request()
{
    GroupCreateRequest *request = malloc(sizeof(GroupCreateRequest));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_CREATE_GROUP);
    printf("创建群输入1, 删除群输入0\n");
    int action;
    scanf("%d", &action);
    printf("请输入群聊名称：\n");
    scanf("%s", request->group_name);
    request->action = htonl(action);
    strncpy(request->session_token, session_token, TOKEN_LEN - 1);
    request->session_token[TOKEN_LEN - 1] = '\0';
    request->length = htonl(sizeof(GroupCreateRequest));
    return request;
}

InviteRequest *build_invite_request()
{
    InviteRequest *request = malloc(sizeof(InviteRequest));
    if (!request)
    {
        perror("内存分配失败");
        exit(1);
    }
    request->request_code = htonl(REQUEST_INVITE_TOGROUP);
    printf("邀请好友进群输入1，删除群成员输入0\n");
    int action;
    scanf("%d", &action);
    printf("请输入群聊名称：\n");
    scanf("%s", request->group_name);
    printf("请输入好友名字：\n");
    scanf("%s", request->friendname);
    request->action = htonl(action);
    strncpy(request->session_token, session_token, TOKEN_LEN - 1);
    request->session_token[TOKEN_LEN - 1] = '\0';
    request->length = htonl(sizeof(InviteRequest));
    return request;
}

HandleGroupInvite *build_handle_group_request()
{
    HandleGroupInvite *request = malloc(sizeof(HandleGroupInvite));

    printf("请输入要处理的群聊名称：\n");
    scanf("%s", request->group_name);
    printf("同意输入1，拒绝0\n");
    int action;
    scanf("%d", &action);

    strncpy(request->session_token, session_token, TOKEN_LEN - 1);
    request->session_token[TOKEN_LEN - 1] = '\0';
    request->request_code = htonl(REQUEST_HANDLE_GROUP);
    request->action = htonl(action);

    request->length = htonl(sizeof(HandleGroupInvite));
    return request;
}

GroupMessage *build_group_message()
{
    GroupMessage *request = malloc(sizeof(GroupMessage));

    request->request_code = htonl(REQUEST_GROUP_MESSAGE);

    printf("请输入群聊 ID：\n");
    unsigned int group_id;
    scanf("%u", &group_id);
    request->group_id = htonl(group_id);

    printf("请输入消息内容：\n");
    scanf("%s", request->message);

    strncpy(request->session_token, session_token, TOKEN_LEN - 1);
    request->session_token[TOKEN_LEN - 1] = '\0';

    request->length = htonl(sizeof(GroupMessage));
    return request;
}

FileTransferRequest *build_file_transfer_req()
{

    FileTransferRequest *request = malloc(sizeof(FileTransferRequest));
    request->request_code = htonl(REQUEST_FILE_TRANSFER);
    request->length = htonl(sizeof(FileTransferRequest));
    printf("请输入好友名称： \n");
    scanf("%s", request->receiver_username);
    printf("请输入文件名称（绝对路径）：\n");
    scanf("%s", request->file_name);
    FILE *file_r;
    file_r = fopen(request->file_name, "r");
    if (file_r == NULL)
    {
        printf(" 没有找到文件，请重新输入文件名(绝对路径)\n");
        scanf("%s", request->file_name);
    }
    strcpy(file_name, request->file_name);
    long file_size = get_file_size(request->file_name);
    request->file_size = htonl((unsigned int)file_size);
    char *filename;
    // 使用 strrchr 找到最后一个斜杠的位置
    filename = strrchr(request->file_name, '/');
    if (filename)
    {
        // 跳过斜杠，指向文件名
        filename++;
        strcpy(request->file_name, filename);
    } // 如果没有找到斜杠，整个路径就是文件名

    strncpy(request->session_token, session_token, TOKEN_LEN - 1);
    request->session_token[TOKEN_LEN - 1] = '\0';
    printf("%s\n", request->file_name);
    return request;
}

GroupNameRestet *build_group_name_reset()
{
    GroupNameRestet *req = (GroupNameRestet *)malloc(sizeof(GroupNameRestet));
    req->request_code = htonl(REQUEST_GROUPNAME_RESET);
    req->length = htonl(sizeof(GroupNameRestet));
    printf(" 请输入群id\n");
    unsigned int id;
    scanf("%u", &id);
    req->group_id = htonl(id);
    printf(" 请输入新的群名称\n");
    scanf("%s", req->group_newname);
    strncpy(req->session_token, session_token, TOKEN_LEN - 1);
    req-> session_token[TOKEN_LEN - 1] = '\0';

    return req;
}

FriendRemarkRequest *build_friend_remark_request()
{
    FriendRemarkRequest *req = (FriendRemarkRequest *)malloc(sizeof(FriendRemarkRequest));
    req->request_code = htonl(REQUEST_FRIEND_REMARK);
    req->length = htonl(sizeof(FriendRemarkRequest));
    strncpy(req->session_token, session_token, TOKEN_LEN - 1);
    req->session_token[TOKEN_LEN - 1] = '\0';
    printf("请输入好友用户名: ");
    scanf("%s", req->friendname);
    printf("请输入好友备注: ");
    scanf("%s", req->remark);
    return req;
}

int file_sock_init()
{
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(FILE_TRANSFER_SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // 创建文件传输套接字
    int file_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (file_sock < 0)
    {
        perror("创建文件传输套接字失败");
        exit(1);
    }

    // 连接文件传输服务器
    if (connect(file_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("连接文件传输服务器失败");
        close(file_sock);
        exit(1);
    }
    return file_sock;
}
int recv_full(int sock, void *buf, size_t len)
{
    size_t total_received = 0;  // 已接收字节数
    ssize_t bytes_received = 0; // 每次调用 recv 接收到的字节数
    while (total_received < len)
    {
        bytes_received = recv(sock, (char *)buf + total_received, len - total_received, 0);
        if (bytes_received < 0)
        {
            if (errno == EINTR)
            {
                // 如果是EINTR错误，继续尝试接收
                continue;
            }
            else
            {
                perror("Error in recv");
                return -1; // 接收失败
            }
        }
        else if (bytes_received == 0)
        {
            fprintf(stderr, "Connection closed by peer\n");
            return 0; // 对端关闭连接
        }
        total_received += bytes_received; // 累计已接收字节数
    }
    return 1; // 成功接收完整数据
}
long get_file_size(const char *filename)
{
    FILE *file = fopen(filename, "rb"); // 以二进制只读模式打开文件
    if (!file)
    {
        perror("Error opening file");
        return -1;
    }
    fseek(file, 0, SEEK_END);     // 移动文件指针到文件末尾
    long file_size = ftell(file); // 获取当前文件指针的位置，即文件大小
    fclose(file);                 // 关闭文件
    return file_size;
}

void file_transfer(char *buffer)
{
    long file_size = get_file_size(file_name);
    float num = (float)file_size / (BUFSIZE - 12);
    printf("文件大小：%ld,分为% .2f次传输\n", file_size, num);
    FILE *file = fopen(file_name, "rb");
    int block_number = 1;

    while (1)
    {
        FileTransferResponse *ack = (FileTransferResponse *)buffer;
        ack->block_number = htonl(block_number);
        ack->request_code = htonl(RESPONSE_FILE_ACK);
        ack->length = htonl(12);
        size_t bytes_read;
        bytes_read = fread(buffer + 12, 1, BUFSIZE - 12, file);
        pthread_mutex_lock(&lock);

        ssize_t sent;
        sent = send(client_fd, buffer, bytes_read + 12, 0);

        printf("sent:%ld\n", sent);

        recv(client_fd, ack, 12, 0);
        printf("第%u块已经确认\n", ntohl(ack->block_number));
        pthread_mutex_unlock(&lock);
        block_number++;
        if (feof(file))
        {
            printf("End of file reached.\n");
            break;
        }
    }
    fclose(file);
}

void file_recv(char *buffer, int file_sock)
{
    FileTransferRequest *req = (FileTransferRequest *)buffer;
    unsigned int file_size = ntohl(req->file_size);
    char filename[256];
    strcpy(filename, req->file_name);
    FILE *file = fopen(filename, "wb");
    char buf[BUFSIZE];
    FileTransferResponse *ack = (FileTransferResponse *)buf;
    ack->block_number = htonl(0);
    ack->length = htonl(12);
    ack->request_code = htonl(RESPONSE_FILE_ACK); // 发送确认之后开始传文件
    send(file_sock, ack, 12, 0);
    int total_write = 0;
    while (1)
    {
        ssize_t bytes_recv = recv(file_sock, buf, BUFSIZE, 0);

        size_t bytes_write;
        bytes_write = fwrite(buf + 12, 1, bytes_recv - 12, file);
        total_write += bytes_write;
        printf("total_write%d\n", total_write);
        if (bytes_write == bytes_recv - 12)
        { // 写入成功，发送ack，开始下一轮接收
            ack->block_number = *(unsigned int *)(buf + 8);
            printf("第%u块接收完成\n", ntohl(ack->block_number));
            send(file_sock, ack, 12, 0);
        }
        if (total_write >= file_size)
        {
            printf("接收完毕\n");
            break;
        }
    }
    fclose(file);
}
