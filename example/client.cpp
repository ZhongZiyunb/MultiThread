/*
 * @Descripttion: 
 * @version: 
 * @Author: zzy
 * @Date: 2023-01-13 17:40:58
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-01-13 17:47:57
 */


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);

  	// 创建并初始化客户端socket地址
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    // 创建客户端socket，客户端使用操作系统自动分配的socket地址，因此无需像服务端那样执行bind操作
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    // 客户端主动发起连接
    if (connect(sockfd, (const sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        printf("connection failed!\n");
    }
    else
    {
        const char* oob_data = "abc";
        const char* normal_data = "123";
        send(sockfd, normal_data, strlen(normal_data), 0);  // 将normal_data处的数据发向客户端socket
        send(sockfd, oob_data, strlen(oob_data), MSG_OOB);
        send(sockfd, normal_data, strlen(normal_data), 0);
    }

    close(sockfd);
    
    return 0;
}
