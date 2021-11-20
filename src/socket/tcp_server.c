/**
 * @file tcp_server.c
 * @date 2020.7.6
 * @details
 */

# include <stdio.h>
# include <string.h>

# include "../utils/myutil.h"
# include "../main/common_def.h"
# include "tcp_server.h"
# include "sock_funcs.h"

/**
 * 初始化p_user指向的用户结构体,用户名设为username,密码设为password,
 * 在线状态设为false,套接字设为INVALID_SOCKET
 * @param p_user 要初始化的用户的地址
 * @param username 用户名
 * @param password 密码
 */
static void init_user(User* p_user, char* username, char* password);

/**
 * 处理登录请求,根据用户传递的用户名和密码验证是否可以成功登录,
 * 并给用户发送相应的报文
 * @param sock 客户端套接字
 */
static void login(SOCKET sock);

/**
 * 处理私聊请求,将收到的私聊消息发送给接收者
 * @param sock 客户端套接字
 */
static void private(SOCKET sock);

/**
 * 处理群发请求,对所有不是信息发送者的在线用户群发收到的群聊消息
 * @param sock 客户端套接字
 */
static void group(SOCKET sock);

/**
 * 将在线用户的列表发送给所有在线用户
 */
static void send_userinfo();

/**
 * 处理登出请求,将该用户的在线状态改为false,并关闭该用户的套接字
 * @param sock 客户端套接字
 */
static void logout(SOCKET sock);

// ----------------------------------------------------------------------------

// 初始化用户列表
void init_users(void) {
    // 此时服务器还没有创建线程,因此不需要互斥量保护
    init_user(users, "Edward", "1234");
    init_user(users + 1, "Winry", "4321");
    init_user(users + 2, "Alphonse", "5678");
    init_user(users + 3, "ZhangMay", "8765");
    
    user_cnt = 4;

    return;
}

// 处理客户请求的线程
void* handle_client_thread(void* arg) {
    SOCKET sock = *((SOCKET*) arg); // 获取客户套接字
    char op[MAX_OP_LEN + 1];
    int len;    // 读取的字符数

    // 从输入流中不断读取报文
    while (1) {
        // 读取操作名
        len = read_line(sock, op);
        op[len] = '\0';

        if (! strcmp(op, LOGIN)) {
            // 登录
            login(sock);
        } else if (! strcmp(op, PRIV)) {
            // 私聊
            private(sock);
        } else if (! strcmp(op, GROUP_)) {
            // 群聊
            group(sock);
        } else if (! strcmp(op, EXIT)) {
            // 登出
            logout(sock);
            return NULL;    // 结束线程
        } else {
            // 报文格式错误
            error_handling("client() error!");
        }
    }
}

// --------------------------------------------------------------------------------

// 初始化用户
static void init_user(User* p_user, char* username, char* password) {
    strcpy(p_user -> username, username);
    strcpy(p_user -> password, password);
    p_user -> is_online = false;
    p_user -> sock = INVALID_SOCKET;

    return;
}

// 登录处理
static void login(SOCKET sock) {
    char username[MAX_NAME_LEN + 1];
    char pwd[MAX_PWD_LEN + 1];
    char* msg;  // 服务器发送的消息
    int len;    // 读取的字符数
    int index;  // 该用户的下标

    // 提取用户名与密码
    len = read_line(sock, username);
    username[len] = '\0';
    len = read_line(sock, pwd);
    pwd[len] = '\0';

    send_line(sock, LOGIN, LOGIN_LEN);

    bool is_successful = false; // 是否登录成功

    pthread_mutex_lock(&user_mutex);    // 加锁

    index = find_by_username(users, user_cnt, username);
    if (index == -1) {
        // 该用户不存在
        msg = "0\nnonexistent username";
    } else if (strcmp(users[index].password, pwd)) {
        // 密码错误
        msg = "0\nwrong password";
    } else {
        // 密码正确,登录成功
        msg = "1";
        is_successful = true;
    }
    send_line(sock, msg, strlen(msg));  // 发送登录结果

    pthread_mutex_unlock(&user_mutex);  // 释放锁    

    if (is_successful) {
        // 如果登陆成功,设置用户在线状态与套接字并发送在线用户列表
        users[index].is_online = true;
        users[index].sock = sock;
        send_userinfo();
    }

    return;
}

// 私聊处理
static void private(SOCKET sock) {
    char user_from[MAX_NAME_LEN + 1];   // 消息发送者
    char user_to[MAX_NAME_LEN + 1];     // 消息接收者
    char msg[MAX_MSG_LEN + 1];          // 私聊消息
    int from_len = read_line(sock, user_from);
    int to_len = read_line(sock, user_to);
    int msg_len = read_line(sock, msg);
    int index;  // 接收私聊消息的用户的下标

    user_to[to_len] = '\0';

    // 临界区
    pthread_mutex_lock(&user_mutex);    // 加锁

    index = find_by_username(users, user_cnt, user_to);
    if (index == -1) {
        // 该用户不存在
        send_line(sock, ERROR_, ERROR_LEN);
        send_line(sock, "1", 1);
    } else if (! users[index].is_online) {
        // 用户不在线
        send_line(sock, ERROR_, ERROR_LEN);
        send_line(sock, "2", 1);
    } else {
        // 给接收者所在的客户端发送报文
        SOCKET to_sock = users[index].sock;

        send_line(to_sock, PRIV, PRIV_LEN);
        send_line(to_sock, user_from, from_len);
        send_line(to_sock, msg, msg_len);
    }
    
    pthread_mutex_unlock(&user_mutex);  // 释放锁

    return;
}


// 群聊处理
static void group(SOCKET sock) {
    char username[MAX_NAME_LEN + 1];
    char msg[MAX_MSG_LEN + 1];
    int username_len = read_line(sock, username);
    int msg_len = read_line(sock, msg);

    username[username_len] = '\0';

    // 临界区
    pthread_mutex_lock(&user_mutex);    // 加锁
    
    SOCKET to_sock;

    for (int i = 0; i < user_cnt; i++) {
        // 该用户在线且不是消息发送者
        if (users[i].is_online && strcmp(users[i].username, username)) {
            to_sock = users[i].sock;

            send_line(to_sock, GROUP_, GROUP_LEN);
            send_line(to_sock, username, username_len);
            send_line(to_sock, msg, msg_len);
        }
    }

    pthread_mutex_unlock(&user_mutex);  // 释放锁

    return;
}

// 发送在线用户列表
// TODO
static void send_userinfo(void) {
    int online_users[user_cnt]; // 在线用户的下标列表
    int online_user_cnt;    // 在线用户数

    pthread_mutex_lock(&user_mutex);    // 加锁

    online_user_cnt = find_online_users(online_users, users, user_cnt);
    if (online_user_cnt == 0) {
        return;
    }

    char username_list[online_user_cnt * (MAX_NAME_LEN + 1) + 1];
    int list_len;   // 在线用户列表长度

    // 拼接用户列表
    username_list[0] = '\0';
    for (int i = 0; i < online_user_cnt; i++) {
        strcat(username_list, users[online_users[i]].username);
        strcat(username_list, "\n");
    }
    list_len = strlen(username_list);

    // 给所有在线用户发送报文
    for (int i = 0; i < online_user_cnt; i++) {
        SOCKET to_sock = users[online_users[i]].sock;   // 获取客户端套接字

        send_line(to_sock, USERINFO, USERINFO_LEN);
        send(to_sock, username_list, list_len, 0);
        send_line(to_sock, END, END_LEN);
    }

    pthread_mutex_unlock(&user_mutex);  // 释放锁
}

// 登出处理
static void logout(SOCKET sock) {
    char username[MAX_NAME_LEN];
    int len = read_line(sock, username);
    int index;  // 用户的下标
    
    // 为true:已登录用户进行登出操作,为false:用户登录失败触发登出操作
    bool flag = false;

    username[len] = '\0';
    
    // 临界区
    pthread_mutex_lock(&user_mutex);    // 加锁

    // 此处应该先关闭客户端套接字再将其套接字设为INVALID_SOCKET
    index = find_by_username(users, user_cnt, username);
    closesocket(users[index].sock);

    if (users[index].is_online) {
        flag = true;
        users[index].is_online = false;
        users[index].sock = INVALID_SOCKET;
    }

    pthread_mutex_unlock(&user_mutex);  // 释放锁

    if (flag) {
        // 当登出操作是用户触发的时候发送在线用户列表
        send_userinfo();
    }

    return;
}
