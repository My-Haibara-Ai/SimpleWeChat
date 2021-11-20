/**
 * @file client.c
 * @date 2020.7.6
 * @details ����΢�ŵĿͻ���
 */

# include <pthread.h>

# include "../utils/myutil.h"
# include "../socket/sock_funcs.h"
# include "../socket/tcp_client.h"

/**
 * �����в����÷�:"client <IP> <port>"
 * @param argc
 * @param argv argv[1]Ϊ��������IP��ַ,argv[2]Ϊ�������Ķ˿ں�
 * @return
 */
int main(int argc, char** argv) {
    // ��������в����Ƿ���ȷ
    if (argc != 3) {
        error_handling("Usage: \"client <IP> <port>\"");
    }

    SOCKET serv_sock;
    SOCKADDR_IN serv_addr;
    pthread_t send_t;
    pthread_t recv_t;

    // �����̵�׼������
    serv_sock = init_socket();
    init_addr(&serv_addr, inet_addr(argv[1]), argv[2]);
    connet_to(serv_sock, serv_addr);

    // ��¼
    if (! login(serv_sock)) {
        return 1;
    }

    // �����߳�
    pthread_create(&send_t, NULL, send_thread, (void*) (&serv_sock));
    pthread_create(&recv_t, NULL, recv_thread, (void*) (&serv_sock));
    
    // �ȴ������߳�ִ�����
    pthread_join(send_t, NULL);
    pthread_join(recv_t, NULL);

    finalize(serv_sock);

    return 0;
}
