/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
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

#include <assert.h>
#include "socket.h"

int amumu_socket_open(int sockfd, int hostshort, int backlog, amumu_socket_termios term) {

    char buf[100];
    memset(buf, 0, 100);
    /*create a socket.type is AF_INET,sock_stream*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    term.server_sockaddr.sin_family = AF_INET;
    term.server_sockaddr.sin_port = htons(hostshort);
    term.server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    term.server_len = sizeof (term.server_sockaddr);

    int on;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));
    /*bind a socket or rename a sockt*/
    if (bind(sockfd, (struct sockaddr*) &term.server_sockaddr, term.server_len) == -1) {
        fprintf(stderr, "Socket bind  failed!\n");
        return -1;

    }

    if (listen(sockfd, backlog) == -1) {
        fprintf(stderr, "Socket listen  failed!\n");
        return -1;
    }
    term.client_len = sizeof (term.client_sockaddr);
    return sockfd;
}

void* funcation(void *arg) {
    result * res = (result*) arg;
    pthread_t th_s,th_r;
    while (1) {
        struct sockaddr_in sockaddr; //表示client端的IP地址
        memset(&sockaddr, 0, sizeof (sockaddr));
        socklen_t len = sizeof (sockaddr);
        if ((res->cli = accept(res->fd, (struct sockaddr*) &sockaddr, &len)) == -1) {
            fprintf(stderr, "Socket connect failed!\n");
            exit(EXIT_FAILURE);
        }
        printf("You got a connection from %s\n", inet_ntoa(sockaddr.sin_addr));
     
        pthread_create(&th_s, NULL,res->sfun ,res);
        pthread_create(&th_r, NULL,res->ufun ,res);
        
    }
    return NULL;
}



int amumu_socket_accept(int sockfd,int uartfd,void *sfun,void * ufun) {
    pthread_t _th;
    result res;
    memset(&res, 0, sizeof (result));
    res.fd=sockfd;
    res.sfun=sfun;
    res.ufun=ufun;
    res.uart=uartfd;
    int ret = pthread_create(&_th, NULL,funcation, &res);
    if (ret != 0) {
        printf("Create thread error!\n");
        return -1;
    }
    return 0;
}

int amumu_socket_close(int sockfd) {
    //assert(fd);
    close(sockfd);
    return 0;
}
