/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <assert.h>
#include "uart.h"

/*
 * 安全读写函数
 */

ssize_t safe_write(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n);
}

ssize_t safe_read(int fd, void *vptr, size_t n) {
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = vptr;
    nleft = n;

    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR)//被信号中断
                nread = 0;
            else
                return -1;
        } else
            if (nread == 0)
            break;
        nleft -= nread;
        ptr += nread;
    }
    return (n - nleft);
}

int amumu_uart_open(int fd, const char* uart) {
    assert(uart);
    if ((fd = open(uart, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
        perror("Open UART failed!");
        return -1;
    }
    /*清除串口非阻塞标志*/
    if (fcntl(fd, F_SETFL, 0) < 0) {
        fprintf(stderr, "fcntl failed!\n");
        return -1;
    }
    return fd;
};

int amumu_uart_termios(int fd, int nSpeed, int cFlow, int nBits, char nEvent, int nStop) {
    struct termios options;

    /*获取终端属性*/
    if (tcgetattr(fd, &options) < 0) {
        perror("Cannot get standard input description");
        return -1;
    }


    /*设置输入输出波特率，两者保持一致*/
    switch (nSpeed) {
        case 4800:
            cfsetispeed(&options, B4800);
            cfsetospeed(&options, B4800);
            break;
        case 9600:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
        case 19200:
            cfsetispeed(&options, B19200);
            cfsetospeed(&options, B19200);
            break;
        case 38400:
            cfsetispeed(&options, B38400);
            cfsetospeed(&options, B38400);
            break;
        case 115200:
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);
            break;
        default:
            fprintf(stderr, "Unkown baude!\n");
            return -1;
    }

    /*设置控制模式*/
    options.c_cflag |= CLOCAL; //保证程序不占用串口
    options.c_cflag |= CREAD; //保证程序可以从串口中读取数据

    /*设置数据流控制*/
    switch (cFlow) {
        case 0://不进行流控制
            options.c_cflag &= ~CRTSCTS;
            break;
        case 1://进行硬件流控制
            options.c_cflag |= CRTSCTS;
            break;
        case 2://进行软件流控制
            options.c_cflag |= IXON | IXOFF | IXANY;
            break;
        default:
            fprintf(stderr, "Unkown c_flow!\n");
            return -1;
    }

    /*设置数据位*/
    switch (nBits) {
        case 5:
            options.c_cflag &= ~CSIZE; //屏蔽其它标志位
            options.c_cflag |= CS5;
            break;
        case 6:
            options.c_cflag &= ~CSIZE; //屏蔽其它标志位
            options.c_cflag |= CS6;
            break;
        case 7:
            options.c_cflag &= ~CSIZE; //屏蔽其它标志位
            options.c_cflag |= CS7;
            break;
        case 8:
            options.c_cflag &= ~CSIZE; //屏蔽其它标志位
            options.c_cflag |= CS8;
            break;
        default:
            fprintf(stderr, "Unkown bits!\n");
            return -1;
    }

    /*设置校验位*/
    switch (nEvent) {
            /*无奇偶校验位*/
        case 'n':
        case 'N':
            options.c_cflag &= ~PARENB; //PARENB：产生奇偶位，执行奇偶校验
            options.c_cflag &= ~INPCK; //INPCK：使奇偶校验起作用
            break;
            /*设为空格,即停止位为2位*/
        case 's':
        case 'S':
            options.c_cflag &= ~PARENB; //PARENB：产生奇偶位，执行奇偶校验
            options.c_cflag &= ~CSTOPB; //CSTOPB：使用两位停止位
            break;
            /*设置奇校验*/
        case 'o':
        case 'O':
            options.c_cflag |= PARENB; //PARENB：产生奇偶位，执行奇偶校验
            options.c_cflag |= PARODD; //PARODD：若设置则为奇校验,否则为偶校验
            options.c_cflag |= INPCK; //INPCK：使奇偶校验起作用
            options.c_cflag |= ISTRIP; //ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
            break;
            /*设置偶校验*/
        case 'e':
        case 'E':
            options.c_cflag |= PARENB; //PARENB：产生奇偶位，执行奇偶校验
            options.c_cflag &= ~PARODD; //PARODD：若设置则为奇校验,否则为偶校验
            options.c_cflag |= INPCK; //INPCK：使奇偶校验起作用
            options.c_cflag |= ISTRIP; //ISTRIP：若设置则有效输入数字被剥离7个字节，否则保留全部8位
            break;
        default:
            fprintf(stderr, "Unkown parity!\n");
            return -1;
    }

    /*设置停止位*/
    switch (nStop) {
        case 1:
            options.c_cflag &= ~CSTOPB; //CSTOPB：使用两位停止位
            break;
        case 2:
            options.c_cflag |= CSTOPB; //CSTOPB：使用两位停止位
            break;
        default:
            fprintf(stderr, "Unkown stop!\n");
            return -1;
    }

    /*设置输出模式为原始输出*/
    options.c_oflag &= ~OPOST; //OPOST：若设置则按定义的输出处理，否则所有c_oflag失效

    /*设置本地模式为原始模式*/
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /*
     *ICANON：允许规范模式进行输入处理
     *ECHO：允许输入字符的本地回显
     *ECHOE：在接收EPASE时执行Backspace,Space,Backspace组合
     *ISIG：允许信号
     */

    /*设置等待时间和最小接受字符*/
    options.c_cc[VTIME] = 0; //可以在select中设置
    options.c_cc[VMIN] = 1; //最少读取一个字符

    /*如果发生数据溢出，只接受数据，但是不进行读操作*/
    tcflush(fd, TCIFLUSH);

    /*激活配置*/
    if (tcsetattr(fd, TCSANOW, &options) < 0) {
        perror("tcsetattr failed");
        return -1;
    }

    return 0;
}

int amumu_uart_read(int fd, void *r_buf, size_t len) {
    ssize_t cnt = 0;
    fd_set rfds;
    struct timeval time;

    /*将文件描述符加入读描述符集合*/
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    /*设置超时为15s*/
    time.tv_sec = 20;
    time.tv_usec = 0;

    /*实现串口的多路I/O*/
    int ret = select(fd + 1, &rfds, NULL, NULL, &time);
    switch (ret) {
        case -1:
            fprintf(stderr, "select error!\n");
            return -1;
        case 0:
            fprintf(stderr, "time over!\n");
            return -1;
        default:
            cnt = safe_read(fd, r_buf, len);
            if (cnt == -1) {
                fprintf(stderr, "read error!\n");
                return -1;
            }
            return cnt;
    }
}

int amumu_uart_write(int fd, const char* w_buf, size_t len) {
    ssize_t cnt = 0;
    cnt = safe_write(fd, w_buf, len);
    if (cnt == -1) {
        fprintf(stderr, "write error!\n");
        return -1;
    }
    return cnt;
}

int amumu_uart_close(int sockfd) {
    assert(sockfd);
    close(sockfd);
    return 0;
};

