#include "socket.h"
#include <stdlib.h>
#include <string.h>
int main(int argc,char*argv[]){

	amumu_socket_callback call;
	memset(&call,0,sizeof(amumu_socket_callback));
	amumu_service_socket(7132,5,call);
	amumu_while_socket(call);






	return 0;
}
