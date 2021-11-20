/**
 * @file myutil.c
 * @date 2020.7.6
 * @details 
 */ 

# include <stdio.h>
# include <stdlib.h>

# include "myutil.h"

// ������
void error_handling(char* msg) {
    printf("%s\n", msg);
    exit(-1);
}

// �ӿ���̨�����ַ�
int read_from_console(char* s, int max_len) {
    char ch;
    int len = 0;    // ������ַ���

    // �����������еĿհ��ַ�
    for (;;) {
        ch = getchar();

        if (ch != '\n' && ch != '\t' && ch != ' ') {
            // ���ch���ǿհ��ַ�
            break;
        }
    }

    // �����ַ�
    for (;;) {
        if (ch == '\n') {
            break;
        } else if (len < max_len) {
            s[len++] = ch;
        } 

        ch = getchar();
    }

    return len;
}
