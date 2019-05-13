#ifndef SOCKET_H
#define SOCKET_H
	#define KEY 123
	#define SIZE 1024
	
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/socket.h>
	typedef struct {
		int sockfd;
		socklen_t server_len;
		socklen_t client_len;
		struct  sockaddr_in server_sockaddr;
		struct  sockaddr_in client_sockaddr;
	}amumu_socket_callback;
		
       int amumu_service_socket(int hostshort, int backlog,amumu_socket_callback callback);
	void amumu_while_socket(amumu_socket_callback callback);

#endif
