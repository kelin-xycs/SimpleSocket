﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;

using System.IO;

namespace SimpleSocket
{
    class Program
    {

        static void Main(string[] args)
        {

            Thread thread = new Thread(Listen);
            thread.Start();

            Console.ReadLine();

        }

        static void Listen()
        {

            SocketCpp socket = null;

            SocketCpp clientSocket;

            Thread thread;

            try
            {
                socket = SocketCpp.CreateAndListen("127.0.0.1", "9527");

                while (true)
                {

                    clientSocket = socket.Accept();

                    thread = new Thread(Receive);

                    thread.Start(clientSocket);

                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Listen while error: " + ex.ToString());
            }


            Console.WriteLine("Listen Close.");

                    
            if (socket != null)
            {
                socket.Shutdown(SocketShutdown.Both);
                socket.Close();
            }

            SocketCpp.WSACleanup();
           
        }

        static void Receive(object obj)
        {
            SocketCpp socket = (SocketCpp)obj;

            byte[] bytes;
            int receiveLength;
            int sentLength;

            try
            {
                while (true)
                {
                    bytes = new byte[32];

                    receiveLength = socket.Receive(bytes);

                    //   receiveLength == 0  表示 客户端 连接 已关闭
                    if (receiveLength == 0)
                    {
                        Console.WriteLine("receiveLength == 0");
                        break;
                    }

                    sentLength = socket.Send(new byte[] { 98, 98 });
                }
            }
            catch(Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }


            Console.WriteLine("close socket");

            if (socket != null)
            {
                socket.Shutdown(SocketShutdown.Both);
                socket.Close();
            }

        }
    }
}
