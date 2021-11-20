/**
 * @file tcp_server.c
 * @date 2020.7.6
 * @details
 */

# include <stdio.h>
# include <string.h>

# include "../utils/myutil.h"
# include "../main/common_def.h"
# include "tcp_server.h"
# include "sock_funcs.h"

/**
 * ��ʼ��p_userָ����û��ṹ��,�û�����Ϊusername,������Ϊpassword,
 * ����״̬��Ϊfalse,�׽�����ΪINVALID_SOCKET
 * @param p_user Ҫ��ʼ�����û��ĵ�ַ
 * @param username �û���
 * @param password ����
 */
static void init_user(User* p_user, char* username, char* password);

/**
 * �����¼����,�����û����ݵ��û�����������֤�Ƿ���Գɹ���¼,
 * �����û�������Ӧ�ı���
 * @param sock �ͻ����׽���
 */
static void login(SOCKET sock);

/**
 * ����˽������,���յ���˽����Ϣ���͸�������
 * @param sock �ͻ����׽���
 */
static void private(SOCKET sock);

/**
 * ����Ⱥ������,�����в�����Ϣ�����ߵ������û�Ⱥ���յ���Ⱥ����Ϣ
 * @param sock �ͻ����׽���
 */
static void group(SOCKET sock);

/**
 * �������û����б��͸����������û�
 */
static void send_userinfo();

/**
 * ����ǳ�����,�����û�������״̬��Ϊfalse,���رո��û����׽���
 * @param sock �ͻ����׽���
 */
static void logout(SOCKET sock);

// ----------------------------------------------------------------------------

// ��ʼ���û��б�
void init_users(void) {
    // ��ʱ��������û�д����߳�,��˲���Ҫ����������
    init_user(users, "Edward", "1234");
    init_user(users + 1, "Winry", "4321");
    init_user(users + 2, "Alphonse", "5678");
    init_user(users + 3, "ZhangMay", "8765");
    
    user_cnt = 4;

    return;
}

// ����ͻ�������߳�
void* handle_client_thread(void* arg) {
    SOCKET sock = *((SOCKET*) arg); // ��ȡ�ͻ��׽���
    char op[MAX_OP_LEN + 1];
    int len;    // ��ȡ���ַ���

    // ���������в��϶�ȡ����
    while (1) {
        // ��ȡ������
        len = read_line(sock, op);
        op[len] = '\0';

        if (! strcmp(op, LOGIN)) {
            // ��¼
            login(sock);
        } else if (! strcmp(op, PRIV)) {
            // ˽��
            private(sock);
        } else if (! strcmp(op, GROUP_)) {
            // Ⱥ��
            group(sock);
        } else if (! strcmp(op, EXIT)) {
            // �ǳ�
            logout(sock);
            return NULL;    // �����߳�
        } else {
            // ���ĸ�ʽ����
            error_handling("client() error!");
        }
    }
}

// --------------------------------------------------------------------------------

// ��ʼ���û�
static void init_user(User* p_user, char* username, char* password) {
    strcpy(p_user -> username, username);
    strcpy(p_user -> password, password);
    p_user -> is_online = false;
    p_user -> sock = INVALID_SOCKET;

    return;
}

// ��¼����
static void login(SOCKET sock) {
    char username[MAX_NAME_LEN + 1];
    char pwd[MAX_PWD_LEN + 1];
    char* msg;  // ���������͵���Ϣ
    int len;    // ��ȡ���ַ���
    int index;  // ���û����±�

    // ��ȡ�û���������
    len = read_line(sock, username);
    username[len] = '\0';
    len = read_line(sock, pwd);
    pwd[len] = '\0';

    send_line(sock, LOGIN, LOGIN_LEN);

    bool is_successful = false; // �Ƿ��¼�ɹ�

    pthread_mutex_lock(&user_mutex);    // ����

    index = find_by_username(users, user_cnt, username);
    if (index == -1) {
        // ���û�������
        msg = "0\nnonexistent username";
    } else if (strcmp(users[index].password, pwd)) {
        // �������
        msg = "0\nwrong password";
    } else {
        // ������ȷ,��¼�ɹ�
        msg = "1";
        is_successful = true;
    }
    send_line(sock, msg, strlen(msg));  // ���͵�¼���

    pthread_mutex_unlock(&user_mutex);  // �ͷ���    

    if (is_successful) {
        // �����½�ɹ�,�����û�����״̬���׽��ֲ����������û��б�
        users[index].is_online = true;
        users[index].sock = sock;
        send_userinfo();
    }

    return;
}

// ˽�Ĵ���
static void private(SOCKET sock) {
    char user_from[MAX_NAME_LEN + 1];   // ��Ϣ������
    char user_to[MAX_NAME_LEN + 1];     // ��Ϣ������
    char msg[MAX_MSG_LEN + 1];          // ˽����Ϣ
    int from_len = read_line(sock, user_from);
    int to_len = read_line(sock, user_to);
    int msg_len = read_line(sock, msg);
    int index;  // ����˽����Ϣ���û����±�

    user_to[to_len] = '\0';

    // �ٽ���
    pthread_mutex_lock(&user_mutex);    // ����

    index = find_by_username(users, user_cnt, user_to);
    if (index == -1) {
        // ���û�������
        send_line(sock, ERROR_, ERROR_LEN);
        send_line(sock, "1", 1);
    } else if (! users[index].is_online) {
        // �û�������
        send_line(sock, ERROR_, ERROR_LEN);
        send_line(sock, "2", 1);
    } else {
        // �����������ڵĿͻ��˷��ͱ���
        SOCKET to_sock = users[index].sock;

        send_line(to_sock, PRIV, PRIV_LEN);
        send_line(to_sock, user_from, from_len);
        send_line(to_sock, msg, msg_len);
    }
    
    pthread_mutex_unlock(&user_mutex);  // �ͷ���

    return;
}


// Ⱥ�Ĵ���
static void group(SOCKET sock) {
    char username[MAX_NAME_LEN + 1];
    char msg[MAX_MSG_LEN + 1];
    int username_len = read_line(sock, username);
    int msg_len = read_line(sock, msg);

    username[username_len] = '\0';

    // �ٽ���
    pthread_mutex_lock(&user_mutex);    // ����
    
    SOCKET to_sock;

    for (int i = 0; i < user_cnt; i++) {
        // ���û������Ҳ�����Ϣ������
        if (users[i].is_online && strcmp(users[i].username, username)) {
            to_sock = users[i].sock;

            send_line(to_sock, GROUP_, GROUP_LEN);
            send_line(to_sock, username, username_len);
            send_line(to_sock, msg, msg_len);
        }
    }

    pthread_mutex_unlock(&user_mutex);  // �ͷ���

    return;
}

// ���������û��б�
// TODO
static void send_userinfo(void) {
    int online_users[user_cnt]; // �����û����±��б�
    int online_user_cnt;    // �����û���

    pthread_mutex_lock(&user_mutex);    // ����

    online_user_cnt = find_online_users(online_users, users, user_cnt);
    if (online_user_cnt == 0) {
        return;
    }

    char username_list[online_user_cnt * (MAX_NAME_LEN + 1) + 1];
    int list_len;   // �����û��б���

    // ƴ���û��б�
    username_list[0] = '\0';
    for (int i = 0; i < online_user_cnt; i++) {
        strcat(username_list, users[online_users[i]].username);
        strcat(username_list, "\n");
    }
    list_len = strlen(username_list);

    // �����������û����ͱ���
    for (int i = 0; i < online_user_cnt; i++) {
        SOCKET to_sock = users[online_users[i]].sock;   // ��ȡ�ͻ����׽���

        send_line(to_sock, USERINFO, USERINFO_LEN);
        send(to_sock, username_list, list_len, 0);
        send_line(to_sock, END, END_LEN);
    }

    pthread_mutex_unlock(&user_mutex);  // �ͷ���
}

// �ǳ�����
static void logout(SOCKET sock) {
    char username[MAX_NAME_LEN];
    int len = read_line(sock, username);
    int index;  // �û����±�
    
    // Ϊtrue:�ѵ�¼�û����еǳ�����,Ϊfalse:�û���¼ʧ�ܴ����ǳ�����
    bool flag = false;

    username[len] = '\0';
    
    // �ٽ���
    pthread_mutex_lock(&user_mutex);    // ����

    // �˴�Ӧ���ȹرտͻ����׽����ٽ����׽�����ΪINVALID_SOCKET
    index = find_by_username(users, user_cnt, username);
    closesocket(users[index].sock);

    if (users[index].is_online) {
        flag = true;
        users[index].is_online = false;
        users[index].sock = INVALID_SOCKET;
    }

    pthread_mutex_unlock(&user_mutex);  // �ͷ���

    if (flag) {
        // ���ǳ��������û�������ʱ���������û��б�
        send_userinfo();
    }

    return;
}
