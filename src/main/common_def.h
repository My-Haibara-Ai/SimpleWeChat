/**
 * @file common_def.h
 * @date 2020.7.6
 * @details �ṩ��������ͻ��˹��õĺ궨�塢ȫ�ֱ������塢���ݽṹ�����
 */ 

# ifndef COMMON_DEF_H
# define COMMON_DEF_H

/**
 * ��������ַ���
 */
# define MAX_OP_LEN    8      // ������
# define MAX_NAME_LEN  10     // �û���
# define MAX_PWD_LEN   10     // �û�����
# define MAX_MSG_LEN   100    // ������Ϣ

/**
 * ������������������ĳ���
 */
# define LOGIN      "@login"
# define PRIV       "@private"
# define GROUP_     "@group"
# define EXIT       "@exit"
# define USERINFO   "@userinfo"
# define ERROR_     "@error"
# define LOGIN_LEN      6
# define PRIV_LEN       8
# define GROUP_LEN      6
# define EXIT_LEN       5
# define USERINFO_LEN   9
# define ERROR_LEN      6

# define END "#end"     // ��־���û��б�Ľ���
# define END_LEN 4

# endif