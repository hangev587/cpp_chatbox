#include "server.h"

// 构造函数
server::server(int port, string ip):server_port(port), server_ip(ip){}

// 析构函数
server::~server(){
    for(auto conn:sock_arr)
        close(conn);
    close(server_sockfd);
}

// 服务器开始服务
void server::run(){
    // 定义 sockfd
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // 定义 sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;           // TCP/IP 协议族
    server_sockaddr.sin_port = htons(server_port);  // server_port; // 端口号
    server_sockaddr.sin_addr.s_addr = inet_addr(server_ip.c_str()); // ip 地址

    // bind, 成功返回0, 出错返回-1
    if(bind(server_sockfd, (struct sockaddr *)&server_sockaddr, sizeof(server_sockaddr)) == -1){
        perror("bind"); // 输出错误原因
        exit(1);        // 结束程序
    }

    // listen, 成功返回0, 出错返回-1
    if(listen(server_sockfd, 20) == -1){
        perror("lister");   // 输出错误原因
        exit(1);            // 结束程序
    }

    // 客户端套接字
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    while(true){    // 不断取出新连接并创建子线程为其服务
        int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
        if(conn < 0){
            perror("connect");  // 输出错误原因
            exit(1);            // 结束程序
        }
        cout << "套接字描述符为 " << conn << " 的客户端成功连接。" << endl;
        sock_arr.push_back(conn);
        thread t(server::RecvMsg, conn);    // 创建线程
        t.detach();     // 置为分离状态, 不能用 join, join 会导致主线程阻塞
    }
}

// 子线程工作的静态函数
// 注意·前面不用加static, 否则会编译报错
void server::RecvMsg(int conn){
    char buffer[1000];  // 接收缓冲区
    while(true){    // 不断接收数据
        memset(buffer, 0, sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer), 0);
        // 客户端发送 exit 或者异常结束时, 退出
        if(strcmp(buffer, "exit")==0 || len<=0){
            cout << "套接字描述符为 " << conn << " 的客户端断开连接。" << endl;
            break;
        }
        cout << "收到套接字描述符为 " << conn << " 发来的信息：" << buffer << endl;
    }
}

