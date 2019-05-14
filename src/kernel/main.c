/*
 *  * To change this license header, choose License Headers in Project Properties.
 *   * To change this template file, choose Tools | Templates
 *    * and open the template in the editor.
 *     */

/* 
 *  * File:   main.c
 *   * Author: Administrator
 *    *
 *     * Created on 2019年5月14日, 下午3:02
 *      */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uart.h"
/*
 *  * 
 *   */
static int ret;
static int fd;

int main(int argc, char** argv) {
    printf("wangyonglin \n");


    
    const char *w_buf = "something to write";
    size_t w_len = sizeof(w_buf);

    char r_buf[1024];
    bzero(r_buf,1024);
    
    fd=amumu_uart_open(fd,"/dev/ttyS0");
    if(fd==-1){
        fprintf(stderr,"uart_open error\n");
        return (EXIT_FAILURE);
    }
    if(amumu_uart_termios(fd,115200,0,8,'N',1)==-1){
        fprintf(stderr,"uart set failed!\n");
       return (EXIT_FAILURE);
    }
    ret = amumu_uart_write(fd,w_buf,w_len);
    if(ret == -1)
    {
        fprintf(stderr,"uart write failed!\n");
        exit(EXIT_FAILURE);
    }
     while(1)
    {
        ret = amumu_uart_read(fd,r_buf,1024);
        if(ret == -1)
        {
            fprintf(stderr,"uart read failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    ret = amumu_uart_close(fd);
    if(ret == -1)
    {
        fprintf(stderr,"uart_close error\n");
        exit(EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}


