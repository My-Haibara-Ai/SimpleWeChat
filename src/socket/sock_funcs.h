/**
 * @file sock_funcs.h
 * @date 2020.7.6
 * @details ���ļ��ṩ���׽�����ص�API,���ⲿ���ý���������
 */

# ifndef SOCK_FUNCS_H
# define SOCK_FUNCS_H

# include <winsock2.h>

/**
 * ���º��������쳣ʱ,�����ӡ�쳣��Ϣ��ǿ���˳�����
 */ 

/**
 * ��ʼ���׽��ֿ�,�����ɻ���IPv4������Э������������ӵ��׽���
 * @return ���ɵ��׽���
 */
SOCKET init_socket(void);

/**
 * ʹ��IPv4����Э���еĵ�ַ��,���ݴ��ݵĲ���ip��port��ʼ��
 * p_addrָ��ı�ʾ�����ַ�Ľṹ��
 * @param p_addr �����׽��������ַ�Ľṹ����ڴ��ַ
 * @param ip �������ֽ��򱣴���׽��ֵ�IP��ַ
 * @param port �ַ���,�׽��ֵĶ˿ں�
 */
void init_addr(SOCKADDR_IN* p_addr, unsigned long ip, char* port);

/**
 * ��serv_addr��ʾ�ĵ�ַ��Ϣ�󶨵��������׽���serv_sock�ϲ���serv_sock�ļ���״̬
 * @param serv_sock �������׽���
 * @param serv_addr �������׽��ֵ������ַ
 */
void bind_listen(SOCKET serv_sock, SOCKADDR_IN serv_addr);

/**
 * �������Կͻ��˵���������,���ɿͻ����׽���,����ӡ�ͻ��˵�IP��ַ�Ͷ˿ں�
 * @param serv_sock �������׽���
 * @return ��ͻ��˽���I/O�Ŀͻ����׽���
 */
SOCKET accept_client(SOCKET serv_sock);

/**
 * �ͻ�����serv_sock���������������������
 * @param serv_sock �������׽���
 * @param serv_addr �������׽��ֵ������ַ
 */
void connet_to(SOCKET serv_sock, SOCKADDR_IN serv_addr);

/**
 * ���׽���sock�ж����ַ�,�������з�'\n'��ֹͣ,�������û��з�.
 * ��������ַ������ַ�����line��,�Ҳ����Զ���lineĩβ��ӿ��ַ�'\0'
 * @param sock ��Է�����I/O�������׽���
 * @param line �ַ�����,��Ŷ�����ַ�,�ַ�����Ҫ��֤�㹻��
 * @return ������ַ�����.�������������ĩβ(EOF)�򷵻�-1
 */
int read_line(SOCKET sock, char* line);

/**
 * ���׽���sock�д���line����line_len���ֽڣ���׷��һ�����з�
 * @param sock ��Է�����I/O�������׽���
 * @param line �������ֽ����е���ʼ��ֵַ
 * @param line_len ��������ֽ���
 */
void send_line(SOCKET sock, char* line, int line_len);

/**
 * �ر��׽���sock,�����׽�����ؿ�
 * @param sock ��Է�����I/O�������׽���
 */
void finalize(SOCKET sock);

# endif
