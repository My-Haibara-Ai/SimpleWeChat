/**
 * @file user.c
 * @date 2020.7.6
 * @details 
 */ 

# include <string.h>

# include "user.h"

// 寻找用户
int find_by_username(User users[], int user_num, char* username) {
    for (int i = 0; i < user_num; i++) {
        if (! strcmp(users[i].username, username)) {
            return i;
        }
    }
    
    return -1;
}

int find_online_users(int online_users[], User users[], int user_num) {
    int cnt = 0;    // 在线用户数

    for (int i = 0; i < user_num; i++) {
        if (users[i].is_online) {
            online_users[cnt++] = i;
        }
    }

    return cnt;
}
