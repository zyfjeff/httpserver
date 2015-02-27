/*
 =====================================================================================
        Filename:  socket_util.cpp
     	Description:  一些使用函数
        Version:  1.0
        Created:  27/02/15 05:22:49
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>

//设置fd为非阻塞
void setnonblocking(int fd)
{
	int oldflsg = fcntl(fd,F_GETFL);
	int newflag = oldflsg|O_NONBLOCK;
	int ret = fcntl(fd,F_SETFL,newflag);
	assert(ret != -1);
}

//设置fd地址重用

void setfdreuseaddr(int sockfd)
{	
       int value = 1;
       int ret = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                      &value, sizeof(value));
       assert(ret != -1);
}



