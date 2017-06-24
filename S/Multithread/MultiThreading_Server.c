#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <stdio.h>
#include "handle_message.h"


#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll

#define MAX_BUFFER_SIZE 1540
#define DEFUALT_PORT "12345"

int main(int argc, char **argv)
{
	WSADATA wsaData;
	int result;
	SOCKET server_socket, client_socket;
	SOCKADDR_IN client_addr;

	struct addrinfo *addr = NULL;
	struct addrinfo hints;

	//u_char len_str[5];

	HANDLE handle;
	DWORD threadID;
	struct threadArgs *threadArgs;
	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed: %d\n", result);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo(NULL, DEFUALT_PORT, &hints, &addr);
	if (result != 0) {
		printf("getaddrinfo failed with error: %d\n", result);
		WSACleanup();
		return 1;
	}

	server_socket = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);//ipv4, tcp
	if (server_socket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(addr);
		WSACleanup();
		return 1;
	}

	result = bind(server_socket, addr->ai_addr, (int)addr->ai_addrlen);
	if (result == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(addr);
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}
	freeaddrinfo(addr);

	result = listen(server_socket, SOMAXCONN);
	printf("Server is listening...\n");
	if (result == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}

	for (;;) {

		// Accept a client socket
		client_socket = accept(server_socket, &client_addr, NULL);
		if (client_socket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(server_socket);
			WSACleanup();
			return 1;
		}
		threadArgs = (struct threadArgs *) malloc(sizeof(struct threadArgs));
		threadArgs->client_sock = client_socket;
		threadArgs->client_addr = client_addr;
		printf("Client connects to server...\n");
		handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)handle_message, threadArgs, 0, (LPDWORD)&threadID);
		if (handle == NULL) {
			printf("Create thread failed!\n");
			printf("Error number:%d\n", WSAGetLastError());
			break;
		}
		
	}


	// cleanup
	closesocket(client_socket);
	closesocket(server_socket);
	WSACleanup();

	return 0;
}