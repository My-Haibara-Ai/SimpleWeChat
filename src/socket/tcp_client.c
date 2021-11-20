/**
 * @file tcp_client.c
 * @date 2020.7.6
 * @details
 */

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>

# include "tcp_client.h"
# include "sock_funcs.h"
# include "../utils/myutil.h"

/**
 * 处理用户的登录请求,给服务器发送登录报文
 * @param serv_sock 服务器端套接字
 * @return
 */
static void login_send(SOCKET serv_sock);

/**
 * 从服务器接受登录报文,处理登录结果
 * @param serv_sock 服务器端套接字
 * @return 登陆成功返回1,失败返回0
 */
static int login_recv(SOCKET serv_sock);

/**
 * 处理用户的私聊请求
 * @param serv_sock 服务器端套接字
 * @param line 用户输入的包含接收者用户名的一行字符串
 * @param line_len line的长度(字符数)
 */
static void private_chat(SOCKET serv_sock, char* line, int line_len);

/**
 * 处理用户的群聊请求
 * @param serv_sock 服务器端套接字
 * @param line 字符串,储存该用户输入的群聊消息
 * @param line_len line的长度(字符数)
 */
static void group_chat(SOCKET serv_sock, char* line, int line_len);

/**
 * 打印在线用户列表
 * @param serv_sock 服务器端套接字
 */
static void print_online_users(SOCKET serv_sock);

/**
 * 接收私聊/群聊消息并打印在屏幕上
 * @param serv_sock 服务器端套接字
 * @param is_private 是否是私聊消息
 */
static void message_recv(SOCKET serv_sock, bool is_private);

/**
 * 处理用户的登出请求
 * @param serv_sock 服务器端套接字
 */
static void logout(SOCKET serv_sock);

// --------------------------------------------------------------

// 登录处理
int login(SOCKET serv_sock) {
    login_send(serv_sock);
    
    return login_recv(serv_sock);
}

// 发送信息的线程
void* send_thread(void* arg) {
    SOCKET serv_sock = *((SOCKET*) arg);
    char line[MAX_MSG_LEN + 1]; // 用户输入的第一行字符
    int len;

    while (1) {
        // 该函数保证了用户不可能输入空串
        len = read_from_console(line, MAX_MSG_LEN + 1);
        line[len] = '\0';

        if (! strcmp(line, "exit")) {
            // 如果用户选择退出
            logout(serv_sock);
            return NULL;
        } else if (line[0] == '@') {
            // 用户选择私聊
            private_chat(serv_sock, line, len);
        } else {
            // 用户选择群聊
            group_chat(serv_sock, line, len);
        }
    }    
}

// 接收信息线程
void* recv_thread(void* arg) {
    SOCKET serv_sock = *((SOCKET*) arg);
    char op[MAX_OP_LEN + 1];            // 操作名
    int len;

    // 不断地接收来自服务器的报文
    while (1) {
        // 获取操作名
        len = read_line(serv_sock, op);

        if (len == -1) {
            // 读到了流的末尾,说明服务器关闭了连接
            return NULL;
        }

        op[len] = '\0';

        if (! strcmp(op, USERINFO)) {
            // 打印在线用户列表
            print_online_users(serv_sock);
        } else if (! strcmp(op, ERROR_)) {
            // 私聊异常处理
            char code[1 + 1];

            read_line(serv_sock, code);
            if (code[0] == '1') {
                printf("error: 用户不存在!\n");
            } else if (code[0] == '2') {
                printf("error: 用户不在线!\n");
            } else {
                error_handling("server error");
            }
        } else if (! strcmp(op, PRIV)) {
            // 私聊消息
            message_recv(serv_sock, true);
        } else if (! strcmp(op, GROUP_)) {
            // 群聊消息
            message_recv(serv_sock, false);
        } else {
            error_handling("server error");
        }
    }
}

// ------------------------------------------------------------------------------------------

// 发送登录报文
static void login_send(SOCKET serv_sock) {
    char pwd[MAX_PWD_LEN + 1];  // 用户密码
    int pwd_len;    // 用户密码长度

    printf("输入用户名与密码登录聊天室!\n");
    printf("用户名: ");
    username_len = read_from_console(username, MAX_NAME_LEN);
    printf("密码: ");
    pwd_len = read_from_console(pwd, MAX_PWD_LEN);
    
    // 将用户名与密码封装为报文发送给服务器
    send_line(serv_sock, LOGIN, LOGIN_LEN);
    send_line(serv_sock, username, username_len);   // 用户名
    send_line(serv_sock, pwd, pwd_len); // 密码

    // 保存用户信息
    username[username_len] = '\0';
}

// 接收登录报文
static int login_recv(SOCKET serv_sock) {
    char op[MAX_OP_LEN + 1];
    char code[1 + 1];
    char detail[20 + 1];
    int len;
    
    // 读入操作名
    len = read_line(serv_sock, op);
    op[len] = '\0';
    if (strcmp(op, LOGIN)) {
        // 报文格式错误
        error_handling("server error!");
    }
    
    // 读入状态码
    read_line(serv_sock, code);
    if (code[0] == '1') {
        // 登录成功
        system("cls");  // 清屏
        
        printf("欢迎来到聊天室,开始Happy的聊天吧!\n");
        printf("1 输入\"exit\"退出聊天室.\n");
        printf("2 输入'@'并跟上接收者用户名发送私聊.\n");
        printf("3 直接输入消息发送群聊.\n");
        printf("\n");
        printf("当前用户: %s\n\n", username);

        return 1;
    } else {
        // 读取登录操作的详细结果
        len = read_line(serv_sock, detail);
        detail[len] = '\0';
        printf("login() error: %s\n", detail);
        system("pause");
        
        // TODO 
        logout(serv_sock);

        return 0;
    }
}

// 发送私聊消息
static void private_chat(SOCKET serv_sock, char* line, int line_len) {
    char user_to[MAX_NAME_LEN + 1]; // 消息接收者用户名
    char msg[MAX_MSG_LEN + 1];  // 私聊消息
    int user_to_len;    // 接收者用户名的长度
    int msg_len;    // 私聊消息长度

    // 获取接收者用户名
    user_to_len = (line_len < MAX_NAME_LEN) ? line_len : MAX_NAME_LEN;
    strncpy(user_to, line + 1, user_to_len);

    // 获取私聊消息
    msg_len = read_from_console(msg, MAX_MSG_LEN);

    // 封装为报文发送给服务器
    send_line(serv_sock, PRIV, PRIV_LEN);
    send_line(serv_sock, username, username_len);   // 发送者
    send_line(serv_sock, user_to, user_to_len);     // 接收者
    send_line(serv_sock, msg, msg_len);             // 私聊消息

    return;   
}

// 发送群聊消息
static void group_chat(SOCKET serv_sock, char* line, int line_len) {
    send_line(serv_sock, GROUP_, GROUP_LEN);
    send_line(serv_sock, username, username_len);
    send_line(serv_sock, line, line_len);

    return;
}

// 接收私聊/群聊消息
static void message_recv(SOCKET serv_sock, bool is_private) {
    char user_from[MAX_NAME_LEN + 1];
    char message[MAX_MSG_LEN + 1];
    int len;

    len = read_line(serv_sock, user_from);
    user_from[len] = '\0';
    len = read_line(serv_sock, message);
    message[len] = '\0';

    if (is_private) {
        printf("私 (%s): %s\n", user_from, message);        
    } else {
        printf("[%s]: %s\n", user_from, message);
    }

    return;
}

// 打印在线用户列表
static void print_online_users(SOCKET serv_sock) {
    char name[MAX_NAME_LEN + 1];
    int len;

    printf("users: [");
    while (1) {
        len = read_line(serv_sock, name);
        name[len] = '\0';
        if (! strcmp(name, END)) {
            break;
        } else {
            printf("%s,", name);
        }
    }
    printf("\b]\n");

    return;
}

// 登出处理
static void logout(SOCKET serv_sock) {
    send_line(serv_sock, EXIT, EXIT_LEN);
    send_line(serv_sock, username, username_len);

    return;
}
