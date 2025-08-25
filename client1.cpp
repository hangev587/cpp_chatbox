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
    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);

    // 定义 sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;      // TCP/IP 协议族
    servaddr.sin_port = htons(8023);    // 服务器端口
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // 服务器 IP

    // 连接服务器, 成功返回0, 错误返回-1
    if(connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("connect");
        exit(1);
    }
    cout << "连接服务器成功。" << endl;

    char sendbuf[100];  // 发送缓冲区
    char recvbuf[100];
    while(true){    // 不断发送数据
        memset(sendbuf, 0, sizeof(sendbuf));    // 清空缓冲区
        cin >> sendbuf;     // 接收终端输入, 写入缓冲区
        send(sock_cli, sendbuf, strlen(sendbuf), 0);    // 发送消息
        // 如果发送的是 exit, 退出
        if(strcmp(sendbuf, "exit") == 0){
            cout << "断开和服务器的链接。" << endl;
            break;
        }
    }
    close(sock_cli);
    return 0;
}
