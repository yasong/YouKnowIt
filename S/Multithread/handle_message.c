#include <stdlib.h>
#include <stdio.h>
#include "handle_message.h"


void *handle_message(void *threadArgs)
{
	int client_sock;
	SOCKADDR_IN clt_addr;
	int result;
	int msg_len;
	//FILE *fp;
	char name[15];
	u_char len_str[5];
	u_char recv_buff[MAX_BUFFER_SIZE];
	//int id;
	//id = GetCurrentThreadId;
	
	client_sock = ((struct threadArgs *) threadArgs) ->client_sock;
	clt_addr = ((struct threadArgs *) threadArgs)->client_addr;

	sprintf(name, "%s", inet_ntoa(clt_addr.sin_addr));
	FILE *fp = fopen(name, "w+b");
	for (;;) {
		msg_len = recv(client_sock, recv_buff, MAX_BUFFER_SIZE, 0);
		if (msg_len > 0) {
			printf("Received message length: %d.\n", msg_len);
			//itoa(result, len, 16);
			sprintf(len_str, "%04x", msg_len);
			fwrite(len_str, sizeof(u_char), 4, fp);//message length
			fwrite(recv_buff, sizeof(u_char), msg_len, fp);//message
		}
		else if (msg_len == 0)
		{
			printf("Connection closing...\n");
			break;
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(client_sock);
			fclose(fp);
			WSACleanup();
			return NULL;
		}
	}
	fclose(fp);
	result = shutdown(client_sock, SD_SEND);
	if (result == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(client_sock);
		WSACleanup();
		return NULL;
	}
	return NULL;
}

