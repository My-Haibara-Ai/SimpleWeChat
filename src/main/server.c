/**
 * @file server.h
 * @date 2020.7.6
 * @details ΢�ŷ�����
 */ 

# include <stdio.h>

# include "../utils/myutil.h"
# include "../socket/sock_funcs.h"
# include "../socket/tcp_server.h"

/**
 * �����в����÷�:"server <port>"
 * @param argc
 * @param argv argv[1]ΪΪ����������Ķ˿ں�
 * @return
 */
int main(int argc, char** argv) {
    // ��������в���
    if (argc != 2) {
        error_handling("Usage:\"server <port>\"");
    }

    SOCKET serv_sock;
    SOCKET client_sock;
    SOCKADDR_IN serv_addr;
    pthread_t handle_thread;

    // �����̵�׼������
    serv_sock = init_socket();
    init_addr(&serv_addr, htonl(INADDR_ANY), argv[1]);
    bind_listen(serv_sock, serv_addr);

    printf("server is running...\n");

    // ��ʼ��������
    pthread_mutex_init(&user_mutex, NULL);

    // ��ʼ���û��б�
    init_users();

    // ����ͻ�����
    while (1) {
        // ����ͻ���������
        client_sock = accept_client(serv_sock);

        // ��������ÿͻ�������߳�
        pthread_create(
                &handle_thread, 
                NULL, 
                handle_client_thread, 
                (void*) (&client_sock)  // �˴�Ӧ�ô��ݿͻ��׽���
        );

        // �����߳�
        pthread_detach(handle_thread);
    }

    // ���ٻ�����
    pthread_mutex_destroy(&user_mutex);

    finalize(serv_sock);

    return 0;
}