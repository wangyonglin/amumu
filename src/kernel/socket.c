#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <syslog.h>
#include "socket.h"

int amumu_service_socket(int hostshort, int backlog,amumu_socket_callback callback)
{
    openlog("amumu_service_socket", LOG_CONS | LOG_PID, 0);
    char buf[100];
    memset(buf,0,100);
    /*create a socket.type is AF_INET,sock_stream*/
    callback.sockfd = socket(AF_INET,SOCK_STREAM,0);
    callback.server_sockaddr.sin_family = AF_INET;
    callback.server_sockaddr.sin_port = htons(hostshort);
    callback.server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    callback.server_len = sizeof(callback.server_sockaddr);
    
    int on;
    setsockopt(callback.sockfd, SOL_SOCKET, SO_REUSEADDR,&on,sizeof(on));
    /*bind a socket or rename a sockt*/
    if(bind(callback.sockfd, (struct sockaddr*)&callback.server_sockaddr, callback.server_len)==-1){
	syslog(LOG_ERR, "hello %s","bind error");
        exit(1);
    }

    if(listen(callback.sockfd,backlog)==-1){
    	syslog(LOG_ERR, "hello %s","listen error");
		exit(1);
    }

	
    callback.client_len = sizeof(callback.client_sockaddr);
	closelog(); 
	
    return 0;
}

void amumu_while_socket(amumu_socket_callback call){
		openlog("amumu_while_socket", LOG_CONS | LOG_PID, 0);
		int client_sockfd;
	    while(1) {

			if((client_sockfd = accept(call.sockfd,(struct sockaddr*)&call.server_sockaddr, &call.client_len)) == -1){
				syslog(LOG_ERR, "hello %s","connect error");
				exit(1);
			}
				syslog(LOG_INFO, "received a connection from %s ", inet_ntoa(call.client_sockaddr.sin_addr)); 
				int error = send(client_sockfd, "You have conected the server", strlen("You have conected the server"), 0);
				printf("%d\n", error);
		}
	closelog(); 
}
