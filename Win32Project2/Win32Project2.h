#include "stdafx.h"




struct CreatePara;
struct AcceptPara;
struct ReceivePara;
struct SendPara;

extern "C"
{
	_declspec(dllexport) void getSocket(CreatePara *para);

	_declspec(dllexport) void getAccept(unsigned int ListenSocket, AcceptPara* para);

	_declspec(dllexport) void getReceive(ReceivePara* para, char* bytes);

	_declspec(dllexport) void getSend(SendPara* para, char* bytes);

	_declspec(dllexport) int getShutdown(unsigned int socket, int how);

	_declspec(dllexport) int getClose(unsigned int socket);

	_declspec(dllexport) int getWSACleanup();
	
}