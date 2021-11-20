/**
 * @file user.h
 * @date 2020.7.6
 * @details 定义了表示用户的结构体并提供了操作该结构体的API
 */ 

# ifndef USER_H
# define USER_H

# include <stdbool.h>
# include <winsock2.h>

# include "../main/common_def.h"

/**
 * 登录服务器的用户
 */
typedef struct User {
    char username[MAX_NAME_LEN + 1];    // 用户名
    char password[MAX_PWD_LEN + 1];     // 密码
    bool is_online;  // 用户是否在线，即是否连接了服务器
    SOCKET sock;    // 与该用户所在的客户端进行I/O的客户端套接字
} User;

/**
 * 在users数组中寻找用户名为username的用户
 * @param users 用户数组
 * @param user_num 用户数
 * @param username 字符串,要寻找的用户的用户名
 * @return 如果找到返回该用户的下标,否则返回-1
 */
int find_by_username(User users[], int user_num, char* username);

/**
 * 查找所有在线的用户,将在线用户下标放入数组online_users中
 * @param online_users 放置在线用户下标的数组,长度至少为user_num
 * @param users 用户数组
 * @param user_num 用户名
 * @return 在线用户数
 */
int find_online_users(int online_users[], User users[], int user_num);

# endif