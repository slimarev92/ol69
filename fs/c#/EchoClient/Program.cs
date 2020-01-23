using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using ilrd.WS06;

namespace ilrd.WS06
{
    class Program
    {
        private const int serverPort = 7777;

        static void Main()
        {
            IPAddress serverAddress = NetworkUtils.GetIPV4Address(Dns.GetHostEntry("localhost"));
            IPEndPoint serverEndPoint = new IPEndPoint(serverAddress, serverPort);

            try
            {
                using var clientUdp = new UdpClient();

                for (var i = 0; 10 > i; ++i)
                {
                    var currMessage = $"{DateTime.Now}: Message {i}";
                    byte[] buffer = ASCIIEncoding.ASCII.GetBytes(currMessage);

                    clientUdp.Send(buffer, buffer.Length, serverEndPoint);

                    buffer = clientUdp.Receive(ref serverEndPoint);

                    Console.WriteLine(ASCIIEncoding.ASCII.GetString(buffer));
                }
            }
            catch (SocketException e)
            {
                Console.WriteLine(e.StackTrace);
            }
        }
    }
}
