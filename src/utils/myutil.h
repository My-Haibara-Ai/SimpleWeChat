/**
 * @file myutil.h
 * @date 2020.7.6
 * @details 提供实用的工具API,如错误处理,字符串处理等等
 */ 

# ifndef MYUTIL_H
# define MYUTIL_H

/**
 * 打印错误消息msg(并换行),然后强制退出程序
 * @param
 *      msg 字符串,描述程序的错误
 * @return
 */
void error_handling(char* msg);

/**
 * 跳过缓冲区中的空白字符(空格、换行、回车、tab)
 * 然后从控制台读入字符,最多读入max_len个字符,舍弃多余的字符.
 * 遇到换行符停止读入字符,并且不保存该换行符.
 * 该函数不会在数组s最后加上空字符'\0'.
 * @param
 *      s 保存读入字符的字符数组,长度至少为max_len 
 *      max_len 最多读入的字符数
 * @return 读入的字符数
 */
int read_from_console(char* s, int max_len);

# endif
