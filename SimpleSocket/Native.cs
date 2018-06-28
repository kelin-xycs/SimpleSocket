using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Runtime.InteropServices;

namespace SimpleSocket
{
    class Native
    {



        [DllImport("Ws2_32")]
        public extern static int getaddrinfo(
            string pNodeName,
            string pServiceName,
            IntPtr pHints,
            IntPtr ppResult
         );

        public struct addrinfo
        {
            public int ai_flags;
            public int ai_family;
            public int ai_socktype;
            public int ai_protocol;
            public long ai_addrlen;
            public string ai_canonname;
            public sockaddr ai_addr;
            //public addrinfo ai_next;
        }

        public struct sockaddr
        {
            public ushort sa_family;
            public char[] sa_data;
        }
    }
}
