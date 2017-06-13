#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <stdlib.h>
#include <stdio.h>


#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll

#define MAX_BUFFER_SIZE 1540
#define DEFUALT_PORT "12345"

int main(int argc, char **argv)
{
	WSADATA wsaData;
	int result;
	SOCKET server_socket, client_socket;

	struct addrinfo *addr = NULL;
	struct addrinfo hints;

	u_char *buff;
	u_char len_str[5];
	int buff_len = MAX_BUFFER_SIZE;
	FILE *fp = fopen("packet", "w+b");
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

	// Accept a client socket
	client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		return 1;
	}
	printf("Client connects to server...\n");
	buff = calloc(buff_len, sizeof(char));
	for (;;) {
		result = recv(client_socket, buff, buff_len, 0);
		if (result > 0) {
			printf("Received message length: %d.\n", result);
			//itoa(result, len, 16);
			sprintf(len_str, "%04x", result);
			fwrite(len_str, sizeof(u_char), 4, fp);//message length
			fwrite(buff, sizeof(u_char), result, fp);//message
			break;
		}
		else if (result == 0)
		{
			printf("Connection closing...\n");
			break;
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(client_socket);
			WSACleanup();
			return 1;
		}
	}

	result = shutdown(client_socket, SD_SEND);
	if (result == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(client_socket);
		WSACleanup();
		return 1;
	}

	// cleanup
	free(buff);
	fclose(fp);
	closesocket(client_socket);
	closesocket(server_socket);
	WSACleanup();

	return 0;
}