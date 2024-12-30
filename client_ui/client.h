#ifndef CLIENT_H
#define CLIENT_H
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>



#ifdef __cplusplus
extern "C" {
#endif
#define MAX_GROUPS 100      // 最大群聊数量
#define MAX_MEMBERS 10      // 每个群聊的最大成员数量
#define MAX_NAME_LENGTH 64  // 名称最大长度
#define MAX_FRIENDS 10
// 群聊信息结构体
typedef struct {
    char name[MAX_NAME_LENGTH];       // 群聊名称
    int id;                           // 群聊 ID
    char owner[MAX_NAME_LENGTH];      // 群主
    char members[MAX_MEMBERS][MAX_NAME_LENGTH]; // 群成员
    int memberCount;                  // 群成员总人数
} GroupInfo;
typedef struct {
    char name[32];   // 好友名字
    char remark[32]; // 备注
    char status[32]; // 在线状态
} FriendInfo;
extern GroupInfo groups[MAX_FRIENDS];
extern int groupsCount;
extern pthread_mutex_t mutex; // 互斥锁用于控制对共享资源的访问
extern pthread_cond_t cond;
extern pthread_mutex_t loggedui_mutex;
extern pthread_cond_t loggedui_cond;
extern int client_fd;
extern char session_token[64];
extern FriendInfo friendList[10];
extern int friendCount;
#define SERVER_IP "127.0.0.1" // 服务器IP地址
#define SERVER_PORT 10005     // 服务器端口
#define FILE_TRANSFER_SERVER_PORT 10007
#define REQUEST_LOGIN 10001
#define RESPONSE_LOGIN 10021
#define REQUEST_LOGOUT 10002

#define REQUEST_ADD_FRIEND 10003
#define REQUEST_HANDELE_ADD 10011
#define REQUEST_DELETE_FRIEND 10004
#define REQUEST_FRIEND_REMARK 10033

#define REQUEST_INVITE_TOGROUP 10013
#define REQUEST_CREATE_GROUP 10006
#define REQUEST_GROUP_MESSAGE 10007
#define REQUEST_HANDLE_GROUP 10031
#define REQUEST_GROUPNAME_RESET 10032

#define REQUEST_PRIVATE_MESSAGE 10008
#define REQUEST_FILE_TRANSFER 10009
#define RESPONSE_FILE_TRANSFER 10016
#define RESPONSE_FILE_ACK 10017

#define REQUEST_CREATEUSER 10010
#define REQUEST_POLLING 10012

#define RESPONSE_MESSAGE 10040
#define SIMPLE_RESPONSE 10050
#define SUCCESS 200
#define FAIL 500

#define CLIENT_EXIT 10000
#define MAX_FRIENDS 10
#define BUFSIZE 4096
#define TOKEN_LEN 64
#define MAX_USERNAME_LENGTH 32
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    char message[256];
} FeedbackMessage;

// 登录请求结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    char username[32];
    char password[32];
} LoginRequest;
// 客户端退出
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
} ClientExit;

// 创建用户请求结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code;
    char username[32];
    char password[32];
} CreateUser;

// 登录响应结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code;
    unsigned int status_code;    // 200=成功, 401=认证失败
    char session_token[64];      // 会话标识符
    char offline_messages[1024]; // 离线消息
} LoginResponse;

// 添加/删除好友请求结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    char session_token[64];
    char friend_username[32];
    unsigned int action; // 1=添加好友, 0=删除好友
} FriendRequest;

// 处理好友请求
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    unsigned int action;       // 1 接受，0拒绝
    char friend_username[32];
    char session_token[64]; // 会话标识符
} HandleFriendRequest;

// 响应用简单的回复报文
typedef struct
{
    unsigned int length;
    unsigned int request_code;
    unsigned int status_code; // 200=成功, 500=失败
} SimpleResponse;

// 好友备注请求结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    char session_token[64];
    char friendname[32];
    char remark[32]; // 好友备注
} FriendRemarkRequest;
// 响应用简单的回复报文

// 创建群组请求结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code;
    int action;
    char session_token[64];
    char group_name[64];
} GroupCreateRequest, HandleGroupInvite;
// 邀请好友进群的结构,也可用于删除好友
typedef struct
{
    unsigned int length;
    unsigned int request_code;
    int action;
    char friendname[32];
    char session_token[64];
    char group_name[64];
} InviteRequest;

// 群组消息广播结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    unsigned int group_id;
    char session_token[64];
    char message[256];
} GroupMessage;

// 修改群名，只有群主可以修改
typedef struct
{
    char session_token[64];
    char group_newname[64];
    unsigned int length;
    unsigned int request_code; // 请求码
    unsigned int group_id;
} GroupNameRestet;

typedef struct
{
    unsigned int length;
    unsigned int status_code; // 200=成功, 500=失败
    char group_id[64];        // 创建成功的群组ID
} GroupCreateResponse;

// 点对点消息结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    char session_token[64];
    char receiver_username[32];
    char message[256];
} PrivateMessage;

// 响应用简单的回复报文

// 文件传输请求结构体
typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    unsigned int file_size;
    char receiver_username[32];
    char session_token[64];
    char file_name[128];
} FileTransferRequest;

typedef struct
{
    unsigned int length;
    unsigned int request_code; // ack
    unsigned int block_number; // 分块编号
} FileTransferResponse;

typedef struct
{
    unsigned int length;
    unsigned int request_code; // 请求码
    char token[64];            // 用于后续传输
} Polling;
void parseGroupInfo(char *message, GroupInfo *groups, int *groupCount);
void parseFriendList(char* message);
void *receive_response();
void init_client();
int recv_full(int sock, void *buf, size_t len);
void send_polling(void *arg);

// 客户端全局变量


CreateUser *build_create_user_request();
FriendRequest *build_friend_request();
HandleFriendRequest *build_handle_friend_request();
PrivateMessage *build_private_message_request();
GroupCreateRequest *build_group_request();
InviteRequest *build_invite_request();
HandleGroupInvite *build_handle_group_request();
GroupMessage *build_group_message();
FileTransferRequest *build_file_transfer_req();
GroupNameRestet *build_group_name_reset();
FriendRemarkRequest *build_friend_remark_request();
long get_file_size(const char *filename);
void file_recv(char *buffer, int file_sock);
void file_transfer(char *buffer);
void exit_client();
int file_sock_init();
void start_client();

#ifdef __cplusplus
}
#endif
#endif // CLIENT_H
