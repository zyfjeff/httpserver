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
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "config.h"
using namespace std;

#define ERROR_EXIT(msg) do{ perror(msg);exit(EXIT_FAILURE);}while(0)

// 去除空格
#define TRIM(str) while(isspace(*str))str++;


extern char buf_404[];

//分析行的几种状态
enum LINE_STATUS{LINE_BAD = 0,LINE_OK = 1,LINE_NONE = 2,LINE_END=3};

// 请求行的三个项
enum HTTP_STATUS{METHOD = 0,FILENAME = 1,VERSION = 2};

//请求行 还是请求头
enum PARSE_STATUS{REQUEST = 0,HEADERS = 1};

void parse_request(int sockfd,char *buf);

void parse_headers(int sockfd,char *buf);

void deal_headers(int client,char *headers,char *value);

void deal_request(int client,char *method,char *filename,char *version);


//static const char *skip(const char *in) {while (in && *in && (unsigned char)*in<=32) in++; return in;}

/*
 * 	200 ok
 *
 */

static void headers_200(int client,const char *type,unsigned int size)
{
 char buf[1024];
 bzero(buf,sizeof(buf));
 strcpy(buf, "HTTP/1.0 200 OK\r\n");
 send(client, buf, strlen(buf), 0);
 bzero(buf,sizeof(buf));
 strcpy(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 bzero(buf,sizeof(buf));
 sprintf(buf,"Content-Length: %d",size);
 send(client,buf,strlen(buf),0);
 bzero(buf,sizeof(buf));
 sprintf(buf, "Content-Type: %s\r\n",type);
 send(client, buf, strlen(buf), 0);
 bzero(buf,sizeof(buf));
 strcpy(buf, "\r\n");
 send(client, buf, strlen(buf), 0); 
}

/*
 *	404无法访问	
 */
static void headers_404(int client)
{
 char buf[1024];
 bzero(buf,sizeof(buf));
 strcpy(buf, "HTTP/1.0 404 Not Found\r\n");
 send(client, buf, strlen(buf), 0);
 strcpy(buf, SERVER_STRING);
 send(client, buf, strlen(buf), 0);
 sprintf(buf, "Content-Type: text/html\r\n");
 send(client, buf, strlen(buf), 0);
 strcpy(buf, "\r\n");
 send(client, buf, strlen(buf), 0); 
}

static const char * GetType(char *filename)
{
	char *postfix;
	postfix = strchr(filename,'.');
	if (!strcmp(postfix,".html") || !strcmp(postfix,".htm"))	{
		return "text/html; charset=UTF-8";
	} else if (!strcmp(postfix,".jpg") || !strcmp(postfix,".jpeg")) {
		return "image/ipeg";
	} else if(!strcmp(postfix,".css")) {
		return "text/css";
	} else if(!strcmp(postfix,"gif")) {
		return "image/gif";
	} else if(!strcmp(postfix,".png")) {
		return "image/png";
	} else {
		return "text/plain; charset=UTF-8";
	}

}

/*
 *	解析一行数据
 *
 */
LINE_STATUS parse_line(int sockfd,char *buf)
{
	int count = -1;
	char ch;
	int ret=0;
	while(1)
	{
	   begin:
		ret = recv(sockfd,&ch,1,0);
		if (ret > 0) {
			count++;
			buf[count] = ch;
			if(buf[count] == '\n' && buf[count-1] == '\r') { //读取到完整行了
				buf[++count] = '\0';
				return LINE_OK; 
			}
			else
				continue;
		} else if (ret == 0) {
			cout << "client end of" << endl; //读取到不完整的行
			return LINE_BAD;
		} else {	
			if(errno == EINTR) //信号中断
				goto begin;
			else
				ERROR_EXIT("recv:");//接收错误
		}	
				
	}
	return LINE_NONE;	 //读取结束
}

/*
 *  	解析HTPP
 */

void parse_http(int sockfd)
{
	char buf[1024];
	bzero(buf,sizeof(buf));
	if (parse_line(sockfd,buf) == LINE_OK)
		parse_request(sockfd,buf);
	close(sockfd);
/*
	PARSE_STATUS status = REQUEST;
	while(parse_line(sockfd,buf) == LINE_OK)
	{
		cout << buf << endl;
		if (status == REQUEST) {
			parse_request(sockfd,buf);
			bzero(buf,sizeof(buf));
			status = HEADERS;
		} else {
			parse_headers(sockfd,buf);
			bzero(buf,sizeof(buf));
		}
	}
*/
	cout << "end of " << endl;
//	close(sockfd);
}

/*
 * 解析http的请求行，分析其请求方法,请求文件, HTTP协议版本
 */
void parse_request(int sockfd,char *buf)
{
//strtok实现请求行的分析
/*
	const char *split = " ";
	strtok(buf,split);
	char *p = NULL;
	while((p = strtok(NULL,split)))
	{
		cout << p << endl;	
	}
*/
	char method[255];
	char filename[255];
	char version[255];
	HTTP_STATUS status = METHOD;
	bzero(method,sizeof(method));
	bzero(filename,sizeof(filename));
	bzero(version,sizeof(version));
	int count = 0;
	TRIM(buf);//去除头部空格
	while(*buf != '\0') {
		switch(status) {
			case METHOD: {
				if (!isspace(*buf)) {
					method[count] = *buf;
					count++;
					buf++;
				} else {
					method[count] = '\0';
					TRIM(buf);
					count = 0;
					status = FILENAME;
				}
				break;
			}
			case FILENAME: {
				if (!isspace(*buf)) {
					filename[count] = *buf;
					count++;
					buf++;
				} else {
					filename[count] = '\0';
					TRIM(buf);
					count = 0;
					status = VERSION;
				}
				break;
			}
			case VERSION: {
				if (!isspace(*buf)) {
					version[count] = *buf;
					count++;
					buf++;
				} else {
					version[count] = '\0';
					TRIM(buf);
					count = 0;
					status = VERSION;
			       }	
				break;
			}
			default:
				break;
		}
	}
	while(parse_line(sockfd,buf) != LINE_BAD  && (strcmp("\r\n",buf) != 0)) //丢弃头部
		cout << buf << endl;
	deal_request(sockfd,method,filename,version);
//	printf("memthod:%s\n",method);
//	printf("filename:%s\n",filename);
//	printf("version:%s\n",version);

}

/*
 * 	读取文件，返回客户端信息.
 *
 */
void deal_request(int client,char *method,char *filename,char *version)
{	
	int ret = 0;
	if(strcmp(filename,"/") == 0) {
		strcpy(filename,"/index.html");
	}
	cout << filename << endl;
	char path[2048];
	bzero(path,sizeof(path));
	sprintf(path,"%s%s",HTTP_ROOT,filename);
	struct stat filestat;
	ifstream file;
	char output[BUFSIZ];
	bzero(output,sizeof(output));
	if(access(path,F_OK) == 0) {
		//输出200
		stat(path,&filestat);
		headers_200(client,GetType(filename),static_cast<unsigned int>(filestat.st_size));
	} else {
		//输出404
		headers_404(client);
		bzero(path,sizeof(path));
		sprintf(path,"%s%s",HTTP_ROOT,"404.html");
		if (access(path,F_OK) != 0) {
			char *err404 = "<h1>404 Not Found</h1>";
			ret  = send(client,err404,strlen(err404),0);
			perror("send:");
			return;
		}
	}

	int filefd = open(path,O_RDWR);
	char *data = (char *)mmap(0,filestat.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,filefd,0);
	send(client,data,filestat.st_size,0);
	munmap(data,filestat.st_size);
	close(filefd);
	
/*
	file.open(path);
	cout << path << endl;
	while(file >> output)
	{
		cout << output << endl;
		send(client,output,strlen(output),0);
		bzero(output,sizeof(output));
	}
	file.close();
*/
}

void parse_headers(int client,char *buf)
{
	cout << buf << endl;	

}

void deal_headers(int client,char *headers,char *value)
{
	close(client);
}
