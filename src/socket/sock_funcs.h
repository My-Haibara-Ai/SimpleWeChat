/**
 * @file sock_funcs.h
 * @date 2020.7.6
 * @details 该文件提供与套接字相关的API,供外部调用进行网络编程
 */

# ifndef SOCK_FUNCS_H
# define SOCK_FUNCS_H

# include <winsock2.h>

/**
 * 以下函数发生异常时,均会打印异常信息并强制退出程序
 */ 

/**
 * 初始化套接字库,并生成基于IPv4互联网协议族和面向连接的套接字
 * @return 生成的套接字
 */
SOCKET init_socket(void);

/**
 * 使用IPv4网络协议中的地址族,根据传递的参数ip、port初始化
 * p_addr指向的表示网络地址的结构体
 * @param p_addr 保存套接字网络地址的结构体的内存地址
 * @param ip 以网络字节序保存的套接字的IP地址
 * @param port 字符串,套接字的端口号
 */
void init_addr(SOCKADDR_IN* p_addr, unsigned long ip, char* port);

/**
 * 将serv_addr表示的地址信息绑定到服务器套接字serv_sock上并打开serv_sock的监听状态
 * @param serv_sock 服务器套接字
 * @param serv_addr 服务器套接字的网络地址
 */
void bind_listen(SOCKET serv_sock, SOCKADDR_IN serv_addr);

/**
 * 接受来自客户端的连接请求,生成客户端套接字,并打印客户端的IP地址和端口号
 * @param serv_sock 服务器套接字
 * @return 与客户端进行I/O的客户端套接字
 */
SOCKET accept_client(SOCKET serv_sock);

/**
 * 客户端向serv_sock发起与服务器的连接请求
 * @param serv_sock 服务器套接字
 * @param serv_addr 服务器套接字的网络地址
 */
void connet_to(SOCKET serv_sock, SOCKADDR_IN serv_addr);

/**
 * 从套接字sock中读入字符,遇到换行符'\n'则停止,并舍弃该换行符.
 * 将读入的字符存入字符数组line中,且不会自动在line末尾添加空字符'\0'
 * @param sock 与对方进行I/O操作的套接字
 * @param line 字符数组,存放读入的字符,字符数组要保证足够大
 * @return 读入的字符个数.如果读到了流的末尾(EOF)则返回-1
 */
int read_line(SOCKET sock, char* line);

/**
 * 向套接字sock中传输line处的line_len个字节，并追加一个换行符
 * @param sock 与对方进行I/O操作的套接字
 * @param line 待传输字节序列的起始地址值
 * @param line_len 待传输的字节数
 */
void send_line(SOCKET sock, char* line, int line_len);

/**
 * 关闭套接字sock,清理套接字相关库
 * @param sock 与对方进行I/O操作的套接字
 */
void finalize(SOCKET sock);

# endif
