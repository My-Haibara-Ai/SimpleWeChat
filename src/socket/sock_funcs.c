/**
 * @file sock_funcs.c
 * @date 2020.7.6
 * @details 
 */ 

# include <stdio.h>

# include "sock_funcs.h"
# include "../utils/myutil.h"

// 初始化套接字
SOCKET init_socket(void) {
    WSADATA wsa_data;
    SOCKET sock;

    // 初始化套接字相关库
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        error_handling("WSAStartup() error!");
    }

    // 生成套接字
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        error_handling("socket() error!");
    }

    return sock;
}

// 初始化地址
void init_addr(SOCKADDR_IN* p_addr, unsigned long ip, char* port) {
    // 将p_addr指向的结构体所在的内存全部初始化为0
    memset(p_addr, 0, sizeof(SOCKADDR_IN));

    (*p_addr).sin_family = AF_INET; // IPv4地址族
    (*p_addr).sin_addr.s_addr = ip;
    (*p_addr).sin_port = htons(atoi(port)); // 先将port转换为整数,再转换为网络字节序

    return;
}

// 绑定地址并监听
void bind_listen(SOCKET serv_sock, SOCKADDR_IN serv_addr) {
    // 绑定网络地址
    if (bind(serv_sock, (SOCKADDR*) (&serv_addr), sizeof(serv_addr)) 
            == SOCKET_ERROR
    ) {
        error_handling("bind() error!");
    }

    // 打开服务器套接字监听状态,设置监听队列长度为10
    if (listen(serv_sock, 10) == SOCKET_ERROR) {
        error_handling("bind() error!");
    }

    return;
}

// 处理连接请求
SOCKET accept_client(SOCKET serv_sock) {
    SOCKET client_sock; // 客户端套接字
    SOCKADDR_IN client_addr;    // 客户端网络地址
    int client_addr_size = sizeof(client_addr); // 客户端网络地址大小

    // 连接客户端,生成套接字
    client_sock = accept(serv_sock, (SOCKADDR*) (&client_addr), &client_addr_size);
    if (client_sock == INVALID_SOCKET) {
        error_handling("accept() error!");
    }

    // 打印客户端的网络地址
    printf("Connected client IP: %s\n", inet_ntoa(client_addr.sin_addr));
    printf("Connectec client port: %d\n", ntohs(client_addr.sin_port));

    return client_sock;
}

// 发起连接请求
void connet_to(SOCKET serv_sock, SOCKADDR_IN serv_addr) {
    if (connect(serv_sock, (SOCKADDR*) (&serv_addr), sizeof(serv_addr))
            == SOCKET_ERROR
    ) {
        error_handling("connet() error!");
    }

    return;
}

// 从套接字中读取一行数据
int read_line(SOCKET sock, char* line) {
    int len;

    for (int i = 0; ; i++) {
        len = recv(sock, line + i, 1, 0);
        if (len == -1) {
            error_handling("recv() error!");
        } else if (len == 0) {
            return -1;
        } else if (line[i] == '\n') {
            return i;
        }
    }
}

// 向套接字中传输一行数据
void send_line(SOCKET sock, char* line, int line_len) {
    send(sock, line, line_len, 0);
    send(sock, "\n", 1, 0);

    return;
}

// 清理
void finalize(SOCKET sock) {
    closesocket(sock);
    WSACleanup();

    return;
}
