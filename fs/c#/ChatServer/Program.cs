using System;
using System.Text;
using System.IO;
using System.Net;
using System.Net.Sockets;

namespace ilrd.WS06
{
    class Program
    {
        static void Main()
        {
            IPEndPoint endpoint = new IPEndPoint(Dns.GetHostAddresses("192.168.1.17")[0], 7777);

            var server = new TCPChatServer(socket => new TCPClientManager(socket), new TCPLogicManager(), endpoint);

            try
            {
                server.Start();
            }
            catch (SocketException)
            {
                Console.WriteLine("BIG ERROR, ABORTING");
            }
        }
    }
}
