/**
 * @file myutil.h
 * @date 2020.7.6
 * @details �ṩʵ�õĹ���API,�������,�ַ�������ȵ�
 */ 

# ifndef MYUTIL_H
# define MYUTIL_H

/**
 * ��ӡ������Ϣmsg(������),Ȼ��ǿ���˳�����
 * @param
 *      msg �ַ���,��������Ĵ���
 * @return
 */
void error_handling(char* msg);

/**
 * �����������еĿհ��ַ�(�ո񡢻��С��س���tab)
 * Ȼ��ӿ���̨�����ַ�,������max_len���ַ�,����������ַ�.
 * �������з�ֹͣ�����ַ�,���Ҳ�����û��з�.
 * �ú�������������s�����Ͽ��ַ�'\0'.
 * @param
 *      s ��������ַ����ַ�����,��������Ϊmax_len 
 *      max_len ��������ַ���
 * @return ������ַ���
 */
int read_from_console(char* s, int max_len);

# endif
