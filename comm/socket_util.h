/*
 * =====================================================================================
 *
 *       Filename:  socket_util.h
 *
 *    Description:  socket实用函数头文件
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
size_t write(int fd,void *ptr,size_t n);
ssize_t readn(int fd,void *ptr,size_t n);
#endif // end HTTPSERVER_COMM_SOCKET_UTIL_H
