#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "header.h"
#include "capture.h"


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "12345"

int main(int argc, char **argv)
{
	WSADATA wsaData;
	SOCKET conn_socket = INVALID_SOCKET;
	struct addrinfo *addr = NULL,
		*ptr = NULL,
		hints;
	int i = 0;
	char *buff;
	int result;
	int buff_len = DEFAULT_BUFLEN;

	// Validate the parameters
	if (argc != 3) {
		printf("usage: %s server-name message\n", argv[0]);
		return 1;
	}

	// Initialize Winsock
	result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		printf("WSAStartup failed with error: %d\n", result);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	result = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &addr);
	if (result != 0) {
		printf("getaddrinfo failed with error: %d\n", result);
		WSACleanup();
		return 1;
	}

	buff = calloc(buff_len, sizeof(char));
	buff = argv[2];
	// Attempt to connect to an address until one succeeds
	for (ptr = addr; ptr != NULL;ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		conn_socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (conn_socket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		// Connect to server.
		result = connect(conn_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (result == SOCKET_ERROR) {
			closesocket(conn_socket);
			conn_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(addr);

	if (conn_socket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return 1;
	}

	// Send an initial buffer
	for (;;) {
		Sleep(500);
		result = capture_packet(conn_socket);
		if (result == -1)
		{
			break;
		}
		/*result = send(conn_socket, buff, (int)strlen(buff), 0);
		if (result == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(conn_socket);
			WSACleanup();
			return 1;
		}*/
	}
	

	// shutdown the connection since no more data will be sent
	result = shutdown(conn_socket, SD_SEND);
	if (result == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(conn_socket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(conn_socket);
	WSACleanup();

	return 0;
}
