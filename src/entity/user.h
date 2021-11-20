/**
 * @file user.h
 * @date 2020.7.6
 * @details �����˱�ʾ�û��Ľṹ�岢�ṩ�˲����ýṹ���API
 */ 

# ifndef USER_H
# define USER_H

# include <stdbool.h>
# include <winsock2.h>

# include "../main/common_def.h"

/**
 * ��¼���������û�
 */
typedef struct User {
    char username[MAX_NAME_LEN + 1];    // �û���
    char password[MAX_PWD_LEN + 1];     // ����
    bool is_online;  // �û��Ƿ����ߣ����Ƿ������˷�����
    SOCKET sock;    // ����û����ڵĿͻ��˽���I/O�Ŀͻ����׽���
} User;

/**
 * ��users������Ѱ���û���Ϊusername���û�
 * @param users �û�����
 * @param user_num �û���
 * @param username �ַ���,ҪѰ�ҵ��û����û���
 * @return ����ҵ����ظ��û����±�,���򷵻�-1
 */
int find_by_username(User users[], int user_num, char* username);

/**
 * �����������ߵ��û�,�������û��±��������online_users��
 * @param online_users ���������û��±������,��������Ϊuser_num
 * @param users �û�����
 * @param user_num �û���
 * @return �����û���
 */
int find_online_users(int online_users[], User users[], int user_num);

# endif