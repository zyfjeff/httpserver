/*
 =====================================================================================
        Filename:  tcpsocket.cpp
     	Description:  socket基础连接库实现
        Version:  1.0
        Created:  27/02/15 03:03:10
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <sys/types.h>
#include <sys/socket.h> 
#include <assert.h>
#include <stdlib.h>
#include <exception>
#include <stdio.h>
#include <iostream>
#include <arpa/inet.h>
#include "tcpsocket.h"
#include "socket_util.h"

using namespace std;

//任意地址，任意端口，任意backlog
tcpsocket::tcpsocket()
        :ip(nullptr),port(0),backlog(5)
{

}

//任意地址，任意端口
tcpsocket::tcpsocket(int backlog)
        :ip(nullptr),port(0),backlog(backlog)
{
	
}

//任意地址
tcpsocket::tcpsocket(int port,int backlog)
	:ip(nullptr),port(port),backlog(backlog)
{

}

tcpsocket::tcpsocket(const char *ip,const int port,const int backlog)
        :ip(ip),port(port),backlog(backlog)
{
	
}

// setp 1.create socket
// setp 2.bind socket
// setp 3.listen socket
void tcpsocket::tcplisten()
{
	int ret;
	struct sockaddr_in addr;
	//setp 1
	socketfd = socket(PF_INET,SOCK_STREAM,0);
	assert(socketfd != -1);
	
	addr.sin_family = AF_INET;
	if (ip != nullptr) { 
	     ret = inet_pton(PF_INET,ip,&addr.sin_addr);
	     assert(ret != -1);
	} else {
             addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	if(port != 0) {
	     addr.sin_port = htons(port);
	} else {
	     addr.sin_port = htons(0);	
	}
	setfdreuseaddr(socketfd);
	// setp 2
	ret = bind(socketfd,(struct sockaddr*)&addr,sizeof(struct sockaddr_in));
	if (ret != 0)
	{
		// 多此一举
		if (errno == EACCES) {
		    cout << "don't have privilege this addr" << endl;
		    exit(EXIT_FAILURE);
		} else if (errno == EADDRINUSE) { 
		    cout << "The given address is already in use" << endl;
		    exit(EXIT_FAILURE);
		}
		else {
		    perror("bind:");
		    exit(EXIT_FAILURE);
		}
	}
	// setp 3
	ret = listen(socketfd,backlog);
	assert(ret != -1);
}
