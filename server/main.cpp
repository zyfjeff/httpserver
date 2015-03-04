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
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#define ERR_EXIT(msg) do{perror(msg);exit(EXIT_FAILURE);}while(0)
using namespace std;
int parse_http(int sockfd);

void print_exit(int status)
{
	if (WIFEXITED(status)) { //正常退出
		cout << "normal termination exit status" << WEXITSTATUS(status) << endl;
	} else if (WIFSIGNALED(status)) { //子进程异常终止
		cout << "abnormal termination signal number" << WTERMSIG(status) << endl;
	#ifdef WCOREDUMP
		if (WCOREDUMP(status))
			cout << "core file generated" << endl;
	#else
	#endif
		else if(WIFSTOPPED(status)) //子进程暂停
			cout << "child stopped  signal number" << WSTOPSIG(status) << endl;
	} 
}
void sigwait_child(int signum)
{
	pid_t pid;
	int stat;
	while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
	{
		print_exit(stat);	
	}
}

int main()
{
	char addrbuf[1024];
	tcpsocket sock(80,100);
	sock.tcplisten();
	int sockfd = sock.getsockfd();
	struct sockaddr_in clientaddr;
	socklen_t len = sizeof(clientaddr);
	signal(SIGCHLD,sigwait_child); // 回收子进程
	while(1)
	{
		int clientfd = accept(sockfd,(struct sockaddr*)&clientaddr,&len);
		if (clientfd == -1) {
			if(errno == EINTR)
				continue;
			else
				ERR_EXIT("accept error");
		}
		cout <<"access ip:" <<inet_ntop(PF_INET,&clientaddr.sin_addr,
		addrbuf,INET_ADDRSTRLEN)<<endl;
		pid_t childid = fork();
		if(childid == 0) { 
			//子进程执行
			parse_http(clientfd);
			exit(EXIT_SUCCESS);
		} else if(childid == -1){
			ERR_EXIT("fork failed");
		} else
			close(clientfd);
	}
}
