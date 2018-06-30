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

//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27016"

#include "Win32Project2.h"


struct CreatePara
{
	const char *host;
	const char *port;

	unsigned int socket;
	int errorCode;
};
void  getSocket(CreatePara* para)
{
	
	WSADATA wsaData;
	int iResult;

	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;


	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0) 
	{
		para->errorCode = iResult;

		return;
	}


	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(para->host, para->port, &hints, &result);

	if (iResult != 0) 
	{
		para->errorCode = iResult;
		
		return;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	
	if (ListenSocket == INVALID_SOCKET) 
	{
		para->errorCode = WSAGetLastError();
		
		freeaddrinfo(result);
		
		return;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);

	if (iResult == SOCKET_ERROR) 
	{
		para->errorCode = WSAGetLastError();

		freeaddrinfo(result);
		
		return;
	}

	freeaddrinfo(result);


	iResult = listen(ListenSocket, SOMAXCONN);

	if (iResult == SOCKET_ERROR) 
	{
		para->errorCode = WSAGetLastError();
		return;
	}

	para->socket = ListenSocket;
}

struct AcceptPara
{
	unsigned int socket;
	int errorCode;
};
void getAccept(unsigned int ListenSocket, AcceptPara* para)
{
	SOCKET ClientSocket = INVALID_SOCKET;

	ClientSocket = accept(ListenSocket, NULL, NULL);

	if (ClientSocket == INVALID_SOCKET) 
	{
		para->errorCode = WSAGetLastError();
		return;
	}

	para->socket = ClientSocket;
}

struct ReceivePara
{
	unsigned int socket;
	int errorCode;
	int bufferLength;
	int resultLength;
};
void getReceive(ReceivePara* para, char* bytes)
{
	int iResult;
	
	iResult = recv(para->socket, bytes, para->bufferLength, 0);
	
	if (iResult == SOCKET_ERROR) 
	{
		para->errorCode = WSAGetLastError();
		return;
	}

	para->resultLength = iResult;
}

struct SendPara
{
	unsigned int socket;
	int errorCode;
	int bufferLength;
	int resultLength;
};
void getSend(SendPara* para, char* bytes)
{
	int iSendResult;

	iSendResult = send(para->socket, bytes, para->bufferLength, 0);

	if (iSendResult == SOCKET_ERROR) 
	{
		para->errorCode = WSAGetLastError();
		return;
	}

	para->resultLength = iSendResult;
}

int getShutdown(unsigned int socket, int how)
{
	return shutdown(socket, how);
}

int getClose(unsigned int socket)
{
	return closesocket(socket);
}

int getWSACleanup()
{
	return WSACleanup();
}