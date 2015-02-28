/*
 =====================================================================================
        Filename:  httpparse.cpp
     	Description:  解析html请求头
        Version:  1.0
        Created:  27/02/15 07:41:17
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
using namespace std;
#define ERROR_EXIT(msg) do{ perror(msg);exit(EXIT_FAILURE);}while(0)

enum LINE_STATUS{LINE_BAD = 0,LINE_OK = 1,LINE_NONE=2,LINE_END=3};

int parse_line(int sockfd,char *buf)
{
	int count = -1;
	char ch;
	int ret=0;
	while(1)
	{
	   begin:
		ret = recv(sockfd,&ch,1,0);
		if (ret >= 0) {
			count++;
			buf[count] = ch;
			if(buf[count] == '\n' && buf[count-1] == '\r') { //读取到完整行了
				buf[++count] = '\0';
				return LINE_OK; 
			}
			else
				continue;
		} else if (ret == 0) {
			cout << "client end of" << endl;
			return LINE_BAD;
		} else {	
			if(errno == EINTR)
				goto begin;
			else
				ERROR_EXIT("recv:");
		}	
				
	}
	return sockfd;	
}

int parse_http(int sockfd)
{
	char buf[1024];
	bzero(buf,sizeof(buf));
	while(parse_line(sockfd,buf) == LINE_OK)
	{
		cout << buf << endl;
		break;
	}
	return sockfd;
}
