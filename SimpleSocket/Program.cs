using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;

namespace SimpleSocket
{
    class Program
    {
        static void Main(string[] args)
        {
            Para para = new Para();

            para.host = "127.0.0.1";
            para.port = "9527";

            //SocketResult socketResult = SocketCpp.socketabcd(para);

            SocketCpp.getSocket(ref para);

            AcceptPara acceptPara = new AcceptPara();

            SocketCpp.getAccept(para.socket, ref acceptPara);

            ReceivePara receivePara = new ReceivePara();

            receivePara.socket = acceptPara.socket;
            //receivePara.buffer = new byte[32];
            receivePara.bufferLength = 32;

            byte[] bytes = new byte[32];

            SendPara sendPara = new SendPara();

            sendPara.socket = acceptPara.socket;
            sendPara.bufferLength = 2;
            
            byte[] bytes2 = { 98, 98 };

            while(true)
            {
                SocketCpp.getReceive(ref receivePara, bytes);
                SocketCpp.getSend(ref sendPara, bytes2);
            }
            

            
            //SocketResult socketResult = (SocketResult)Marshal.PtrToStructure(p, typeof(SocketResult));

            string a = "";

            //const string DEFAULT_PORT = "27015";

//struct addrinfo *result = NULL, *ptr = NULL, hints;
            //IntPtr pResult , pptr ;
            //Native.addrinfo hints = new Native.addrinfo();

            
            
            //ZeroMemory(&amp;hints, sizeof (hints));
//hints.ai_family = AF_INET;
//hints.ai_socktype = SOCK_STREAM;
//hints.ai_protocol = IPPROTO_TCP;
//hints.ai_flags = AI_PASSIVE;

//// Resolve the local address and port to be used by the server
//iResult = getaddrinfo(NULL, DEFAULT_PORT, &amp;hints, &amp;result);
//if (iResult != 0) {
//    printf("getaddrinfo failed: %d\n", iResult);
//    WSACleanup();
//    return 1;
//}
        }
    }
}
