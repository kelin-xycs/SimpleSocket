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

        //[return: MarshalAs(UnmanagedType.Struct)]
        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        public extern static void getSocket(ref Para para);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        public extern static void getAccept(uint listenSocket, ref AcceptPara para);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        public extern static void getReceive(ref ReceivePara para, byte[] bytes);

        [DllImport("Win32Project2", CallingConvention = CallingConvention.Cdecl)]
        public extern static void getSend(ref SendPara para, byte[] bytes);
    }

    

    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    struct SocketResult
    {
        //[MarshalAs(UnmanagedType.U4)]
        public uint socket;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string errorMsg;

        //[MarshalAs(UnmanagedType.I4)]
        public int errorCode;
    }

    [StructLayout(LayoutKind.Sequential, CharSet=CharSet.Ansi)]
    struct Para
    {
        //[MarshalAs(UnmanagedType.LPWStr)]
        public string host;
        //[MarshalAs(UnmanagedType.LPWStr)]
        public string port;
        //[MarshalAs(UnmanagedType.U4)]
        public uint socket;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string errorMsg;

        //[MarshalAs(UnmanagedType.I4)]
        public int errorCode;
    }

    struct AcceptPara
    {
        public uint socket;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string errorMsg;

        //[MarshalAs(UnmanagedType.I4)]
        public int errorCode;
    }

    struct ReceivePara
    {
        public uint socket;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string errorMsg;

        
        public int errorCode;

        public byte[] buffer;
        public int bufferLength;
        public int resultLength;

        
    }

    struct SendPara
    {
        public uint socket;

        //[MarshalAs(UnmanagedType.LPWStr)]
        public string errorMsg;


        public int errorCode;

        public byte[] buffer;
        public int bufferLength;
        public int resultLength;


    }
}
