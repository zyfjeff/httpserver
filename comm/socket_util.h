/*
 * =====================================================================================
 *
 *       Filename:  socket_util.h
 *
 *    Description:  socket适用函数头文件
 *
 *        Version:  1.0
 *        Created:  27/02/15 05:37:09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jeff (), zyfforlinux@163.com
 *   Organization:  Linux
 *
 * =====================================================================================
 */
#ifndef HTTPSERVER_COMM_SOCKET_UTIL_H
#define HTTPSERVER_COMM_SOCKET_UTIL_H

void setnonblocking(int fd);
void setfdreuseaddr(int sockfd);

#endif // end HTTPSERVER_COMM_SOCKET_UTIL_H
