/**
 * @file tcp_server.h
 * @date 2020.7.6
 * @details ΢�ŷ������õ���ȫ�ֱ��������Լ�����ҵ���߼��ĺ���
 */

# ifndef TCP_SERVER_H
# define TCP_SERVER_H

# include <pthread.h>

# include "../entity/user.h"

# define MAX_CLIENT_SIZE 20 // ����û���

int user_cnt;   // �û���
User users[MAX_CLIENT_SIZE];    // �û��б�
pthread_mutex_t user_mutex; // ��������user_cnt��users�Ļ����� 

/**
 * ��ʼ��users�б�,���һЩ�û�
 */
void init_users(void);

/**
 * ����ͻ�������߳�
 * @param arg ��ͻ��˽���I/O�������׽��ֵĵ�ַ
 */
void* handle_client_thread(void* arg);

# endif