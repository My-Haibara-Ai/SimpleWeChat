cd ../src

: 编译客户端
gcc -o ../bin/client socket/sock_funcs.c socket/tcp_client.c utils/myutil.c main/client.c -lpthread -lwsock32

: 编译服务器端
gcc -o ../bin/server entity/user.c socket/sock_funcs.c socket/tcp_server.c utils/myutil.c main/server.c -lpthread -lwsock32

cd ../bin
