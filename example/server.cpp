/*
 * @Descripttion: 
 * @version: 
 * @Author: zzy
 * @Date: 2023-01-13 17:40:50
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2023-01-13 17:56:38
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);
	
	// 创建并初始化服务端socket地址
    sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
	
	// 1. 创建服务端socket	SOCK_STREAM：流服务，TCP协议
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
	
	// 2. 将socket与socket地址绑定
    int ret = bind(sock, (const sockaddr*)&address, sizeof(address));
    assert(ret != -1);
	
	// 3. 监听socket（等待客户端的连接）
    ret = listen(sock, 5);
    assert(ret != -1);
	
	// 创建客户端socket地址（用于记录客户端的socket地址，其值在accept接受连接时确认）
    sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);
	
	// 4. 服务端被动连接
    int connfd = accept(sock, (struct sockaddr* )&client, &client_addrlength);	 	// connfd：连接socket，connfd是一个新生成的socket，与sock不是一回事
    if (connfd < 0)
    {
        printf("errno is: %d\n", errno);
    }
    else
    {
        char buffer[BUF_SIZE];

        memset(buffer, '\0', BUF_SIZE);
        ret = recv(connfd, buffer, BUF_SIZE - 1, 0);      // 从socket中读取数据，放置在buffer中
        printf("got %d bytes of normal data '%s'\n", ret, buffer);

        memset(buffer, '\0', BUF_SIZE);
        ret = recv(connfd, buffer, BUF_SIZE - 1, MSG_OOB);
        printf("got %d bytes of oob data '%s'\n", ret, buffer);

        memset(buffer, '\0', BUF_SIZE);
        ret = recv(connfd, buffer, BUF_SIZE - 1, 0);
        printf("got %d bytes of normal data '%s'\n", ret, buffer);

        close(connfd);
    }
    
    close(sock);
    
    return 0;
}
