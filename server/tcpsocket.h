/*
 * =====================================================================================
 *
 *       Filename:  tcp_scoket.h
 *
 *    Description:  tcp socket封装
 *
 *        Version:  1.0
 *        Created:  07/02/15 10:27:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jeff (), zyfforlinux@163.com
 *   Organization:  Linux
 *
 * =====================================================================================
 */

#ifndef HTTPSERVER_SERVER_TCPSOCKET_H
#define HTTPSERVER_SERVER_TCPSOCKET_H

class tcpsocket
{
public:
	tcpsocket();
	tcpsocket(int backlog);
	tcpsocket(int port,int backlog);
	tcpsocket(const char *ip,const int port,const int backlog);
	void tcplisten();
	int getsockfd()
	{
		return socketfd;
	}
private:
	const char *ip;
	const int port;
	int socketfd;
	const int backlog;
	
};
#endif //end of tcpsocket.h
