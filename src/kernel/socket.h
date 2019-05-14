/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   socket.h
 * Author: Administrator
 *
 * Created on 2019年5月14日, 下午5:34
 */

#ifndef SOCKET_H
#define SOCKET_H

#ifdef __cplusplus
extern "C" {
#endif

#define KEY 123
#define SIZE 1024

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

    typedef struct {
        socklen_t server_len;
        socklen_t client_len;
        struct sockaddr_in server_sockaddr;
        struct sockaddr_in client_sockaddr;
    } amumu_socket_termios;


    typedef struct {
        int fd;
        void *sfun;
        void *ufun;
        int cli;
        int uart;
        pthread_t *thr;
    } result;

    int amumu_socket_open(int sockfd, int hostshort, int backlog, amumu_socket_termios term);
    int amumu_socket_accept(int sockfd,int uartfd,void *sfun,void * ufun);
    int amumu_socket_close(int sockfd);


#ifdef __cplusplus
}
#endif

#endif /* SOCKET_H */

