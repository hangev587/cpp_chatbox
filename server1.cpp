#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <iostream>

using namespace std;

int main(){
    // 定义 sockfd
    int server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 定义 sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;   // TCP/IP 协议族
    server_sockaddr.sin_port = htons(8023); // 端口号
    server_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");   // IP 地址

    // bind, 成功返回0，出错返回-1
    if(bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    // listen, 成功返回0，出错返回-1
    if(listen(server_sockfd, 20) == -1){
        perror("listen");
        exit(1);
    }

    // 客户端套接字
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    // 成功返回非负描述字, 出错返回-1
    int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
    if(conn < 0){
        perror("connect");
        exit(1);
    }
    cout << "客户端成功连接。\n" << endl;

    char buffer[1000];  // 接收缓冲区

    while(true){    // 不断接收数据
        memset(buffer, 0, sizeof(buffer));  // 清空缓冲区
        int len = recv(conn, buffer, sizeof(buffer), 0);
        // 客户端发送 exit 或者异常结束时, 退出
        if(strcmp(buffer, "exit")==0 || len<=0)
            break;
        cout << "收到客户信息：" << buffer << endl;
    }
    close(conn);
    close(server_sockfd);
    return 0;
}

