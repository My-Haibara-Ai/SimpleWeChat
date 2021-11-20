/**
 * @file tcp_server.h
 * @date 2020.7.6
 * @details 微信服务器用到的全局变量定义以及处理业务逻辑的函数
 */

# ifndef TCP_SERVER_H
# define TCP_SERVER_H

# include <pthread.h>

# include "../entity/user.h"

# define MAX_CLIENT_SIZE 20 // 最大用户数

int user_cnt;   // 用户数
User users[MAX_CLIENT_SIZE];    // 用户列表
pthread_mutex_t user_mutex; // 保护变量user_cnt与users的互斥量 

/**
 * 初始化users列表,添加一些用户
 */
void init_users(void);

/**
 * 处理客户请求的线程
 * @param arg 与客户端进行I/O操作的套接字的地址
 */
void* handle_client_thread(void* arg);

# endif