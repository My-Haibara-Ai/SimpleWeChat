/**
 * @file myutil.c
 * @date 2020.7.6
 * @details 
 */ 

# include <stdio.h>
# include <stdlib.h>

# include "myutil.h"

// 错误处理
void error_handling(char* msg) {
    printf("%s\n", msg);
    exit(-1);
}

// 从控制台读入字符
int read_from_console(char* s, int max_len) {
    char ch;
    int len = 0;    // 读入的字符数

    // 跳过缓冲区中的空白字符
    for (;;) {
        ch = getchar();

        if (ch != '\n' && ch != '\t' && ch != ' ') {
            // 如果ch不是空白字符
            break;
        }
    }

    // 读入字符
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
