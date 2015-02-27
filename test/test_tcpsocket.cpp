/*
 =====================================================================================
        Filename:  test_tcpsocket.cpp
     	Description:  
        Version:  1.0
        Created:  27/02/15 04:41:42
        Revision:  none
        Compiler:  gcc
        Author:  Jeff (), zyfforlinux@163.com
    	Organization:  Linux
 =====================================================================================
 */

#include <tcpsocket.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h> 

using namespace std;

int main()
{
	char buf[BUFSIZ];
	tcpsocket tcp(80,100);
	tcp.tcplisten();
	int fd =  accept(tcp.getsockfd(),NULL,NULL);
	int ret = read(fd,buf,BUFSIZ-1);
	cout << buf << endl;
}
