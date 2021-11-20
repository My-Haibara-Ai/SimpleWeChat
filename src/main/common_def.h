/**
 * @file common_def.h
 * @date 2020.7.6
 * @details 提供服务器与客户端共用的宏定义、全局变量定义、数据结构定义等
 */ 

# ifndef COMMON_DEF_H
# define COMMON_DEF_H

/**
 * 定义最大字符数
 */
# define MAX_OP_LEN    8      // 操作名
# define MAX_NAME_LEN  10     // 用户名
# define MAX_PWD_LEN   10     // 用户密码
# define MAX_MSG_LEN   100    // 聊天消息

/**
 * 定义操作名及操作名的长度
 */
# define LOGIN      "@login"
# define PRIV       "@private"
# define GROUP_     "@group"
# define EXIT       "@exit"
# define USERINFO   "@userinfo"
# define ERROR_     "@error"
# define LOGIN_LEN      6
# define PRIV_LEN       8
# define GROUP_LEN      6
# define EXIT_LEN       5
# define USERINFO_LEN   9
# define ERROR_LEN      6

# define END "#end"     // 标志着用户列表的结束
# define END_LEN 4

# endif