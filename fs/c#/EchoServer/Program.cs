using System.Net;
using System.Net.Sockets;
using System.Text;

namespace ilrd.WS06
{
    public static class NetworkUtils
    {
        public static IPAddress GetIPV4Address(IPHostEntry entry)
        {
            IPAddress retVal = null;

            foreach (IPAddress address in entry.AddressList)
            {
                if (address.AddressFamily.Equals(AddressFamily.InterNetwork))
                {
                    retVal = address;
                }
            }

            return retVal;
        }
    }

    class Program
    {
        private const int serverPort = 7777;

        static void Main(string[] args)
        {
            IPHostEntry serverHostEntry = Dns.GetHostEntry("localhost");
            IPAddress serverIP = NetworkUtils.GetIPV4Address(serverHostEntry);
            var endpoint = new IPEndPoint(serverIP, serverPort);
            using var serverUdpClient = new UdpClient(endpoint);

            try
            {
                System.Console.WriteLine("Waiting for messages");

                while (true)
                {
                    IPEndPoint senderEndpoint = new IPEndPoint(IPAddress.Any, 0);
                    byte[] buffer = serverUdpClient.Receive(ref senderEndpoint);

                    System.Console.WriteLine(Encoding.Default.GetString(buffer, 0, buffer.Length));

                    serverUdpClient.Send(buffer, buffer.Length, senderEndpoint);
                }
            }
            catch (SocketException e)
            {
                System.Console.WriteLine(e.StackTrace);
            }
        }
    }
}
