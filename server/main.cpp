/*
 =====================================================================================
        Filename:  main.cpp
     	Description:  http服务器
        Version:  1.0
        Created:  28/02/15 02:16:11
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include "tcpsocket.h"
#include <sys/types.h>          /*  See NOTES */
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

using namespace std;
int parse_http(int sockfd);
int main()
{
	char addrbuf[1024];
	tcpsocket sock(80,100);
	sock.tcplisten();
	int sockfd = sock.getsockfd();
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	while(1)
	{
		int clientfd = accept(sockfd,(struct sockaddr*)&clientaddr,&len);
		cout <<"access ip:" <<inet_ntop(PF_INET,&clientaddr.sin_addr,
		addrbuf,INET_ADDRSTRLEN)<<endl;
		parse_http(clientfd);
	}
}
