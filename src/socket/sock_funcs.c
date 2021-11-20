/**
 * @file sock_funcs.c
 * @date 2020.7.6
 * @details 
 */ 

# include <stdio.h>

# include "sock_funcs.h"
# include "../utils/myutil.h"

// ��ʼ���׽���
SOCKET init_socket(void) {
    WSADATA wsa_data;
    SOCKET sock;

    // ��ʼ���׽�����ؿ�
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        error_handling("WSAStartup() error!");
    }

    // �����׽���
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        error_handling("socket() error!");
    }

    return sock;
}

// ��ʼ����ַ
void init_addr(SOCKADDR_IN* p_addr, unsigned long ip, char* port) {
    // ��p_addrָ��Ľṹ�����ڵ��ڴ�ȫ����ʼ��Ϊ0
    memset(p_addr, 0, sizeof(SOCKADDR_IN));

    (*p_addr).sin_family = AF_INET; // IPv4��ַ��
    (*p_addr).sin_addr.s_addr = ip;
    (*p_addr).sin_port = htons(atoi(port)); // �Ƚ�portת��Ϊ����,��ת��Ϊ�����ֽ���

    return;
}

// �󶨵�ַ������
void bind_listen(SOCKET serv_sock, SOCKADDR_IN serv_addr) {
    // �������ַ
    if (bind(serv_sock, (SOCKADDR*) (&serv_addr), sizeof(serv_addr)) 
            == SOCKET_ERROR
    ) {
        error_handling("bind() error!");
    }

    // �򿪷������׽��ּ���״̬,���ü������г���Ϊ10
    if (listen(serv_sock, 10) == SOCKET_ERROR) {
        error_handling("bind() error!");
    }

    return;
}

// ������������
SOCKET accept_client(SOCKET serv_sock) {
    SOCKET client_sock; // �ͻ����׽���
    SOCKADDR_IN client_addr;    // �ͻ��������ַ
    int client_addr_size = sizeof(client_addr); // �ͻ��������ַ��С

    // ���ӿͻ���,�����׽���
    client_sock = accept(serv_sock, (SOCKADDR*) (&client_addr), &client_addr_size);
    if (client_sock == INVALID_SOCKET) {
        error_handling("accept() error!");
    }

    // ��ӡ�ͻ��˵������ַ
    printf("Connected client IP: %s\n", inet_ntoa(client_addr.sin_addr));
    printf("Connectec client port: %d\n", ntohs(client_addr.sin_port));

    return client_sock;
}

// ������������
void connet_to(SOCKET serv_sock, SOCKADDR_IN serv_addr) {
    if (connect(serv_sock, (SOCKADDR*) (&serv_addr), sizeof(serv_addr))
            == SOCKET_ERROR
    ) {
        error_handling("connet() error!");
    }

    return;
}

// ���׽����ж�ȡһ������
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

// ���׽����д���һ������
void send_line(SOCKET sock, char* line, int line_len) {
    send(sock, line, line_len, 0);
    send(sock, "\n", 1, 0);

    return;
}

// ����
void finalize(SOCKET sock) {
    closesocket(sock);
    WSACleanup();

    return;
}
