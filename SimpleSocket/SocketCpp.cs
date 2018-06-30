using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;

namespace SimpleSocket
{
    class SocketCpp
    {

        private uint socket;

        private SocketCpp(uint socket)
        {
            this.socket = socket;
        }

        public static SocketCpp CreateAndListen(string host, string port)
        {
            CreatePara para = new CreatePara();

            para.host = host;
            para.port = port;

            SocketCpp.getSocket(ref para);

            SimpleSocketException ex;

            if (para.errorCode != 0)
            {
                ex = new SimpleSocketException(
                    "CreateAndListen 发生错误， errorCode：" + para.errorCode,
                    para.errorCode);
        
                throw ex;
            }

            return new SocketCpp(para.socket);
        }

        public SocketCpp Accept()
        {
            AcceptPara para = new AcceptPara();

            SocketCpp.getAccept(this.socket, ref para);

            SimpleSocketException ex;

            if (para.errorCode != 0)
            {
                ex = new SimpleSocketException(
                    "Accept 发生错误， errorCode：" + para.errorCode,
                    para.errorCode);

                throw ex;
            }

            return new SocketCpp(para.socket);
        }

        public int Receive(byte[] buffer)
        {
            ReceivePara para = new ReceivePara();

            para.socket = this.socket;
            para.bufferLength = buffer.Length;

            SocketCpp.getReceive(ref para, buffer);

            SimpleSocketException ex;

            if (para.errorCode != 0)
            {
                ex = new SimpleSocketException(
                    "Receive 发生错误， errorCode：" + para.errorCode,
                    para.errorCode);

                throw ex;
            }

            return para.resultLength;
        }

        public int Send(byte[] buffer)
        {
            SendPara para = new SendPara();

            para.socket = this.socket;
            para.bufferLength = buffer.Length;

            SocketCpp.getSend(ref para, buffer);

            SimpleSocketException ex;

            if (para.errorCode != 0)
            {
                ex = new SimpleSocketException(
                    "Send 发生错误， errorCode：" + para.errorCode,
                    para.errorCode);

                throw ex;
            }

            return para.resultLength;
        }

        public int Shutdown(SocketShutdown how)
        {
            return getShutdown(this.socket, (int)how);
        }

        public int Close()
        {
            return getClose(this.socket);
        }

        public static int WSACleanup()
        {
            return getWSACleanup();
        }

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        extern static void getSocket(ref CreatePara para);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        extern static void getAccept(uint listenSocket, ref AcceptPara para);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        extern static void getReceive(ref ReceivePara para, byte[] bytes);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        extern static void getSend(ref SendPara para, byte[] bytes);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        extern static int getShutdown(uint socket, int how);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        extern static int getClose(uint socket);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        extern static int getWSACleanup();
    }

    enum SocketShutdown
    {
        Receive,
        Send,
        Both
    }

    //[StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi)]
    struct CreatePara
    {
        public string host;
        public string port;
        public uint socket;
        public int errorCode;
    }

    struct AcceptPara
    {
        public uint socket;
        public int errorCode;
    }

    struct ReceivePara
    {
        public uint socket;
        public int errorCode;
        public int bufferLength;
        public int resultLength;
    }

    struct SendPara
    {
        public uint socket;
        public int errorCode;
        public int bufferLength;
        public int resultLength;
    }
}
