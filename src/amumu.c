#include <stdio.h>
#include "kernel/bst.h"
#include "kernel/socket.h"
#include "kernel/boot.h"
#include "kernel/uart.h"
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include <memory.h>

#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include "kernel/uart.h"
#include "kernel/socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>



void * send_funcation(void *arg) {
    result *p = (result*) arg;
    char buf[1024];

    while (1) {
        memset(buf, 0, sizeof (buf));
        int rc = amumu_uart_read(p->uart, buf, 1024);
        if (rc <= 0)//如果recv返回小于等于0，代表socket已经关闭或者出错了
            break;
        send(p->cli, buf, strlen(buf), 0);

    }
    return NULL;
}

void *recv_funcation(void *arg)//接收client端socket数据的线程
{
    result *p = (result*) arg;
    int uartfd = p->uart;
    char buf[1024];
    while (1) {
        memset(buf, 0, sizeof (buf));
        int rc = recv(p->cli, buf, 1024, 0);
        if (rc <= 0)//如果recv返回小于等于0，代表socket已经关闭或者出错了
            break;
        amumu_uart_write(uartfd, buf, strlen(buf));
        send(p->cli, buf, strlen(buf), 0);
    }
    pthread_exit(NULL);
    return NULL;
}

int main(int argc,char* argv[]){
	
	boot();
	int uartfd;
    int sockfd;
    char r_buf[1024];
    bzero(r_buf, 1024);
    char w_buf[1024];
    bzero(w_buf, 1024);
    amumu_socket_termios term;
    memset(&term, 0, sizeof (amumu_socket_termios));

    uartfd = amumu_uart_open(uartfd, "/dev/ttyS0");
    if (uartfd == -1) {
        fprintf(stderr, "Open uart failed!\n");
        exit(EXIT_FAILURE);
    }

    if (amumu_uart_termios(uartfd, 115200, 0, 8, 'N', 1) == -1) {
        fprintf(stderr, "Set uart failed!\n");
        exit(EXIT_FAILURE);
    }

    sockfd = amumu_socket_open(sockfd, 7132, 5, term);
    if (sockfd == -1) {
        fprintf(stderr, "Socket initialize failed!\n");
        exit(EXIT_FAILURE);
    }

    amumu_socket_accept(sockfd, uartfd, recv_funcation, send_funcation);
    while (1);

    int ret = amumu_uart_close(uartfd);
    if (ret == -1) {
        fprintf(stderr, "amumu_uart_close error\n");
        exit(EXIT_FAILURE);
    }
    ret = amumu_socket_close(sockfd);
    if (ret == -1) {
        fprintf(stderr, "amumu_socket_close error\n");
        exit(EXIT_FAILURE);
    }
    printf("EXIT_SUCCESS \n");
    return (EXIT_SUCCESS);
	return 0;
}
