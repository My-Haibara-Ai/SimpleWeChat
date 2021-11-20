/**
 * @file tcp_client.h
 * @date 2020.7.6
 * @details ΢�ſͻ����õ���ȫ�ֱ��������Լ�����ҵ���߼��ĺ���
 */

# ifndef TCP_CLIENT_H
# define TCP_CLIENT_H

# include <winsock2.h>

# include "../main/common_def.h"

char username[MAX_NAME_LEN + 1];    // ��ǰ��¼�û����û���
int username_len;   // �û������ַ���

/**
 * �����û��ĵ�¼����,��¼�ɹ�����ʾ�û��ѳɹ����������ҵ���Ϣ,
 * ������ʾ��¼ʧ�ܵ���Ϣ
 * @param serv_sock �������׽���
 * @return ��½�ɹ�����1,ʧ�ܷ���0
 */
int login(SOCKET serv_sock);

/**
 * ��������������ͱ��ĵ��߳�
 * @param arg �������˽��ֵĵ�ַ
 * @return
 */
void* send_thread(void* arg);

/**
 * ����������������ı��ĵ��߳�
 * @param arg ���������׽��ֵĵ�ַ
 * @return
 */
void* recv_thread(void* arg);

# endif
