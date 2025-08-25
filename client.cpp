#include "client.h"

client::client(int port, string ip):server_port(port), server_ip(ip){}

client::~client(){
    close(sock);
}

void client::run(){
    // 定义sockfd
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 定义 sockaddr_in
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port); // 服务器端口
    servaddr.sin_addr.s_addr = inet_addr(server_ip.c_str());    // 服务器 ip

    // 连接服务器, 成功返回0, 错误返回-1
    if(connect(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("connect");
        exit(1);
    }
    cout << "连接服务器成功。" << endl;

    // 创建发送线程和接收线程
    thread send_t(SendMsg, sock), recv_t(RecvMsg, sock);
    send_t.join();
    cout << "发送线程已结束。" << endl;
    recv_t.join();
    cout << "接收线程已结束。" << endl;

    return;
}

// 注意，前面不用加 static !
void client::SendMsg(int conn){
    char sendbuf[100];  // 发送缓冲区
    while(true){
        memset(sendbuf, 0, sizeof(sendbuf));
        cin >> sendbuf;
        int ret = send(conn, sendbuf, strlen(sendbuf), 0);  // 发送
        // 输入 exit 或者对端关闭时结束
        if(strcmp(sendbuf, "exit")==0 || ret<=0){
            cout << "和服务器断开连接..." << endl;
            break;
        }
    }
}

void client::RecvMsg(int conn){
    char buffer[1000];  // 接收缓冲区
    while(true){        // 不断接收数据
        memset(buffer, 0, sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer), 0);
        if(len <= 0)    // recv 返回值小于等于0, 退出
            break;
        cout << "收到服务器发来的信息：" << buffer << endl;
    }
}
