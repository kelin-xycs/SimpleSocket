// Win32Project2.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"

#include "Win32Project2.h"

struct Para
{
	const char *host;
	const char *port;
	unsigned int socket = 0;

	const char *errorMsg;
	int errorCode = 0;
};

//struct SocketResult
//{
//	unsigned int socket = 0;
//
//	const char *errorMsg;
//	int errorCode = 0;	
//};


void  getSocket(Para* para)
{
	//SocketResult socketResult;
	//printf("errorCode: %d\n", socketResult.errorCode);
	//socketResult.errorCode = 100;
	//return &socketResult;
	//printf("errorCode: %d\n", socketResult.errorCode);
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		
		para->errorMsg = "WSAStartup failed with error: ";
		para->errorCode = iResult;
		//errorMsg = "WSAStartup failed with error: ";
		//*errorCode = iResult;
		//return &socketResult;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	//iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	iResult = getaddrinfo(NULL, para->port, &hints, &result);
	if (iResult != 0) {
		//printf("getaddrinfo failed with error: %d\n", iResult);
		para->errorMsg = "getaddrinfo failed with error: ";
		para->errorCode = iResult;
		WSACleanup();
		//return 1;
		//return &socketResult;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		para->errorMsg = "socket failed with error: ";
		para->errorCode = WSAGetLastError();
		//printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		//return 1;
		//return &socketResult;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		para->errorMsg = "bind failed with error: ";
		para->errorCode = WSAGetLastError();
		//printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		//return &socketResult;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		para->errorMsg = "listen failed with error: ";
		para->errorCode = WSAGetLastError();
		//printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		//return &socketResult;
	}

	//printf("errorCode: %d\n", socketResult.errorCode);
	//printf("errorCode: %d\n", socketResult.socket);
	para->socket = ListenSocket;
	//printf("errorCode: %d\n", socketResult.socket);
	//return &socketResult;
}

struct AcceptPara
{
	unsigned int socket = 0;

	const char *errorMsg;
	int errorCode = 0;
};

void getAccept(unsigned int ListenSocket, AcceptPara* para)
{
	
	SOCKET ClientSocket = INVALID_SOCKET;

	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {

		para->errorMsg = "accept failed with error: ";
		para->errorCode = WSAGetLastError();
		//printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		//return 1;
	}

	para->socket = ClientSocket;
}

struct ReceivePara
{
	unsigned int socket = 0;

	const char *errorMsg;
	int errorCode = 0;

	char *buffer;
	int bufferLength = 0;
	int resultLength = 0;
};
void getReceive(ReceivePara* para, char* bytes)
{
	int iResult;
	char buff[32]; 
	//iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
	//iResult = recv(para->socket, buff, 32, 0);
	iResult = recv(para->socket, bytes, para->bufferLength, 0);
	if (iResult > 0) {
		//printf("Bytes received: %d\n", iResult);

		
	}
	else if (iResult == 0)
		printf("Connection closing...\n");
	else  {
		printf("recv failed with error: %d\n", WSAGetLastError());
		//closesocket(ClientSocket);
		para->errorMsg = "recv failed with error: ";
		para->errorCode = WSAGetLastError();
		closesocket(para->socket);
		WSACleanup();
		//return 1;
	}

	para->resultLength = iResult;
}

struct SendPara
{
	unsigned int socket = 0;

	const char *errorMsg;
	int errorCode = 0;

	char *buffer;
	int bufferLength = 0;
	int resultLength = 0;
};
void getSend(SendPara* para, char* bytes)
{
	int iSendResult;

	//iSendResult = send(ClientSocket, recvbuf, iResult, 0);
	iSendResult = send(para->socket, bytes, para->bufferLength, 0);
	if (iSendResult == SOCKET_ERROR) {
		para->errorMsg = "send failed with error: ";
		para->errorCode = WSAGetLastError();
		//printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(para->socket);
		WSACleanup();
		//return 1;
	}
	para->resultLength = iSendResult;
	//printf("Bytes sent: %d\n", iSendResult);
}