/**
 * @file server.h
 * @date 2020.7.6
 * @details 微信服务器
 */ 

# include <stdio.h>

# include "../utils/myutil.h"
# include "../socket/sock_funcs.h"
# include "../socket/tcp_server.h"

/**
 * 命令行参数用法:"server <port>"
 * @param argc
 * @param argv argv[1]为为服务器分配的端口号
 * @return
 */
int main(int argc, char** argv) {
    // 检查命令行参数
    if (argc != 2) {
        error_handling("Usage:\"server <port>\"");
    }

    SOCKET serv_sock;
    SOCKET client_sock;
    SOCKADDR_IN serv_addr;
    pthread_t handle_thread;

    // 网络编程的准备工作
    serv_sock = init_socket();
    init_addr(&serv_addr, htonl(INADDR_ANY), argv[1]);
    bind_listen(serv_sock, serv_addr);

    printf("server is running...\n");

    // 初始化互斥量
    pthread_mutex_init(&user_mutex, NULL);

    // 初始化用户列表
    init_users();

    // 处理客户请求
    while (1) {
        // 受理客户连接请求
        client_sock = accept_client(serv_sock);

        // 创建处理该客户请求的线程
        pthread_create(
                &handle_thread, 
                NULL, 
                handle_client_thread, 
                (void*) (&client_sock)  // 此处应该传递客户套接字
        );

        // 销毁线程
        pthread_detach(handle_thread);
    }

    // 销毁互斥量
    pthread_mutex_destroy(&user_mutex);

    finalize(serv_sock);

    return 0;
}