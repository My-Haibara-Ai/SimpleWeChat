/**
 * @file tcp_client.c
 * @date 2020.7.6
 * @details
 */

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <stdbool.h>

# include "tcp_client.h"
# include "sock_funcs.h"
# include "../utils/myutil.h"

/**
 * �����û��ĵ�¼����,�����������͵�¼����
 * @param serv_sock ���������׽���
 * @return
 */
static void login_send(SOCKET serv_sock);

/**
 * �ӷ��������ܵ�¼����,�����¼���
 * @param serv_sock ���������׽���
 * @return ��½�ɹ�����1,ʧ�ܷ���0
 */
static int login_recv(SOCKET serv_sock);

/**
 * �����û���˽������
 * @param serv_sock ���������׽���
 * @param line �û�����İ����������û�����һ���ַ���
 * @param line_len line�ĳ���(�ַ���)
 */
static void private_chat(SOCKET serv_sock, char* line, int line_len);

/**
 * �����û���Ⱥ������
 * @param serv_sock ���������׽���
 * @param line �ַ���,������û������Ⱥ����Ϣ
 * @param line_len line�ĳ���(�ַ���)
 */
static void group_chat(SOCKET serv_sock, char* line, int line_len);

/**
 * ��ӡ�����û��б�
 * @param serv_sock ���������׽���
 */
static void print_online_users(SOCKET serv_sock);

/**
 * ����˽��/Ⱥ����Ϣ����ӡ����Ļ��
 * @param serv_sock ���������׽���
 * @param is_private �Ƿ���˽����Ϣ
 */
static void message_recv(SOCKET serv_sock, bool is_private);

/**
 * �����û��ĵǳ�����
 * @param serv_sock ���������׽���
 */
static void logout(SOCKET serv_sock);

// --------------------------------------------------------------

// ��¼����
int login(SOCKET serv_sock) {
    login_send(serv_sock);
    
    return login_recv(serv_sock);
}

// ������Ϣ���߳�
void* send_thread(void* arg) {
    SOCKET serv_sock = *((SOCKET*) arg);
    char line[MAX_MSG_LEN + 1]; // �û�����ĵ�һ���ַ�
    int len;

    while (1) {
        // �ú�����֤���û�����������մ�
        len = read_from_console(line, MAX_MSG_LEN + 1);
        line[len] = '\0';

        if (! strcmp(line, "exit")) {
            // ����û�ѡ���˳�
            logout(serv_sock);
            return NULL;
        } else if (line[0] == '@') {
            // �û�ѡ��˽��
            private_chat(serv_sock, line, len);
        } else {
            // �û�ѡ��Ⱥ��
            group_chat(serv_sock, line, len);
        }
    }    
}

// ������Ϣ�߳�
void* recv_thread(void* arg) {
    SOCKET serv_sock = *((SOCKET*) arg);
    char op[MAX_OP_LEN + 1];            // ������
    int len;

    // ���ϵؽ������Է������ı���
    while (1) {
        // ��ȡ������
        len = read_line(serv_sock, op);

        if (len == -1) {
            // ����������ĩβ,˵���������ر�������
            return NULL;
        }

        op[len] = '\0';

        if (! strcmp(op, USERINFO)) {
            // ��ӡ�����û��б�
            print_online_users(serv_sock);
        } else if (! strcmp(op, ERROR_)) {
            // ˽���쳣����
            char code[1 + 1];

            read_line(serv_sock, code);
            if (code[0] == '1') {
                printf("error: �û�������!\n");
            } else if (code[0] == '2') {
                printf("error: �û�������!\n");
            } else {
                error_handling("server error");
            }
        } else if (! strcmp(op, PRIV)) {
            // ˽����Ϣ
            message_recv(serv_sock, true);
        } else if (! strcmp(op, GROUP_)) {
            // Ⱥ����Ϣ
            message_recv(serv_sock, false);
        } else {
            error_handling("server error");
        }
    }
}

// ------------------------------------------------------------------------------------------

// ���͵�¼����
static void login_send(SOCKET serv_sock) {
    char pwd[MAX_PWD_LEN + 1];  // �û�����
    int pwd_len;    // �û����볤��

    printf("�����û����������¼������!\n");
    printf("�û���: ");
    username_len = read_from_console(username, MAX_NAME_LEN);
    printf("����: ");
    pwd_len = read_from_console(pwd, MAX_PWD_LEN);
    
    // ���û����������װΪ���ķ��͸�������
    send_line(serv_sock, LOGIN, LOGIN_LEN);
    send_line(serv_sock, username, username_len);   // �û���
    send_line(serv_sock, pwd, pwd_len); // ����

    // �����û���Ϣ
    username[username_len] = '\0';
}

// ���յ�¼����
static int login_recv(SOCKET serv_sock) {
    char op[MAX_OP_LEN + 1];
    char code[1 + 1];
    char detail[20 + 1];
    int len;
    
    // ���������
    len = read_line(serv_sock, op);
    op[len] = '\0';
    if (strcmp(op, LOGIN)) {
        // ���ĸ�ʽ����
        error_handling("server error!");
    }
    
    // ����״̬��
    read_line(serv_sock, code);
    if (code[0] == '1') {
        // ��¼�ɹ�
        system("cls");  // ����
        
        printf("��ӭ����������,��ʼHappy�������!\n");
        printf("1 ����\"exit\"�˳�������.\n");
        printf("2 ����'@'�����Ͻ������û�������˽��.\n");
        printf("3 ֱ��������Ϣ����Ⱥ��.\n");
        printf("\n");
        printf("��ǰ�û�: %s\n\n", username);

        return 1;
    } else {
        // ��ȡ��¼��������ϸ���
        len = read_line(serv_sock, detail);
        detail[len] = '\0';
        printf("login() error: %s\n", detail);
        system("pause");
        
        // TODO 
        logout(serv_sock);

        return 0;
    }
}

// ����˽����Ϣ
static void private_chat(SOCKET serv_sock, char* line, int line_len) {
    char user_to[MAX_NAME_LEN + 1]; // ��Ϣ�������û���
    char msg[MAX_MSG_LEN + 1];  // ˽����Ϣ
    int user_to_len;    // �������û����ĳ���
    int msg_len;    // ˽����Ϣ����

    // ��ȡ�������û���
    user_to_len = (line_len < MAX_NAME_LEN) ? line_len : MAX_NAME_LEN;
    strncpy(user_to, line + 1, user_to_len);

    // ��ȡ˽����Ϣ
    msg_len = read_from_console(msg, MAX_MSG_LEN);

    // ��װΪ���ķ��͸�������
    send_line(serv_sock, PRIV, PRIV_LEN);
    send_line(serv_sock, username, username_len);   // ������
    send_line(serv_sock, user_to, user_to_len);     // ������
    send_line(serv_sock, msg, msg_len);             // ˽����Ϣ

    return;   
}

// ����Ⱥ����Ϣ
static void group_chat(SOCKET serv_sock, char* line, int line_len) {
    send_line(serv_sock, GROUP_, GROUP_LEN);
    send_line(serv_sock, username, username_len);
    send_line(serv_sock, line, line_len);

    return;
}

// ����˽��/Ⱥ����Ϣ
static void message_recv(SOCKET serv_sock, bool is_private) {
    char user_from[MAX_NAME_LEN + 1];
    char message[MAX_MSG_LEN + 1];
    int len;

    len = read_line(serv_sock, user_from);
    user_from[len] = '\0';
    len = read_line(serv_sock, message);
    message[len] = '\0';

    if (is_private) {
        printf("˽ (%s): %s\n", user_from, message);        
    } else {
        printf("[%s]: %s\n", user_from, message);
    }

    return;
}

// ��ӡ�����û��б�
static void print_online_users(SOCKET serv_sock) {
    char name[MAX_NAME_LEN + 1];
    int len;

    printf("users: [");
    while (1) {
        len = read_line(serv_sock, name);
        name[len] = '\0';
        if (! strcmp(name, END)) {
            break;
        } else {
            printf("%s,", name);
        }
    }
    printf("\b]\n");

    return;
}

// �ǳ�����
static void logout(SOCKET serv_sock) {
    send_line(serv_sock, EXIT, EXIT_LEN);
    send_line(serv_sock, username, username_len);

    return;
}
