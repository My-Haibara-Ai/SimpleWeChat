/**
 * @file tcp_client.h
 * @date 2020.7.6
 * @details 微信客户端用到的全局变量定义以及处理业务逻辑的函数
 */

# ifndef TCP_CLIENT_H
# define TCP_CLIENT_H

# include <winsock2.h>

# include "../main/common_def.h"

char username[MAX_NAME_LEN + 1];    // 当前登录用户的用户名
int username_len;   // 用户名的字符数

/**
 * 处理用户的登录请求,登录成功则显示用户已成功进入聊天室的信息,
 * 否则显示登录失败的信息
 * @param serv_sock 服务器套接字
 * @return 登陆成功返回1,失败返回0
 */
int login(SOCKET serv_sock);

/**
 * 负责给服务器发送报文的线程
 * @param arg 服务器端接字的地址
 * @return
 */
void* send_thread(void* arg);

/**
 * 负责处理服务器发来的报文的线程
 * @param arg 服务器端套接字的地址
 * @return
 */
void* recv_thread(void* arg);

# endif
