#include <stdlib.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>


#define MAX_BUFFER_SIZE 1540
struct threadArgs{
	int client_sock;
	SOCKADDR_IN client_addr;
};
void *handle_message(void *threadArgs);