/**
 * @file client.c
 * @date 2020.7.6
 * @details 简易微信的客户端
 */

# include <pthread.h>

# include "../utils/myutil.h"
# include "../socket/sock_funcs.h"
# include "../socket/tcp_client.h"

/**
 * 命令行参数用法:"client <IP> <port>"
 * @param argc
 * @param argv argv[1]为服务器的IP地址,argv[2]为服务器的端口号
 * @return
 */
int main(int argc, char** argv) {
    // 检查命令行参数是否正确
    if (argc != 3) {
        error_handling("Usage: \"client <IP> <port>\"");
    }

    SOCKET serv_sock;
    SOCKADDR_IN serv_addr;
    pthread_t send_t;
    pthread_t recv_t;

    // 网络编程的准备工作
    serv_sock = init_socket();
    init_addr(&serv_addr, inet_addr(argv[1]), argv[2]);
    connet_to(serv_sock, serv_addr);

    // 登录
    if (! login(serv_sock)) {
        return 1;
    }

    // 创建线程
    pthread_create(&send_t, NULL, send_thread, (void*) (&serv_sock));
    pthread_create(&recv_t, NULL, recv_thread, (void*) (&serv_sock));
    
    // 等待两个线程执行完毕
    pthread_join(send_t, NULL);
    pthread_join(recv_t, NULL);

    finalize(serv_sock);

    return 0;
}
