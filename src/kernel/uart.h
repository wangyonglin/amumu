/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   uart.h
 * Author: Administrator
 *
 * Created on 2019年5月14日, 下午5:30
 */

#ifndef UART_H
#define UART_H

#ifdef __cplusplus
extern "C" {
#endif

int amumu_uart_pthread(int uartfd,int sockfd ,void * fun);


#ifdef __cplusplus
}
#endif

#endif /* UART_H */

