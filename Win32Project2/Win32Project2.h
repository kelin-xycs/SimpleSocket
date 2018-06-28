#include "stdafx.h"

struct Para;


struct SocketResult;

struct AcceptPara;
struct ReceivePara;
struct SendPara;

extern "C"
{
	_declspec(dllexport) void getSocket(Para *para);
	//typedef void (*ApiGetSocket)(Para*);

	_declspec(dllexport) void getAccept(unsigned int ListenSocket, AcceptPara* para);
	//typedef void(*ApiAccept)(unsigned int, AcceptPara*);

	_declspec(dllexport) void getReceive(ReceivePara* para, char* bytes);

	_declspec(dllexport) void getSend(SendPara* para, char* bytes);
}