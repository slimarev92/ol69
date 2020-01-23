using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ilrd.WS06
{
    class MulticastEchoServer : IDisposable
    {
        private readonly UdpClient multicastClient;
        private readonly IPEndPoint multicastEndpoint;

        public MulticastEchoServer(string ip, int port)
        {
            IPAddress multicastIp = Dns.GetHostAddresses(ip)[0];
            multicastEndpoint = new IPEndPoint(multicastIp, port);

            try
            {
                multicastClient = new UdpClient(port);
                multicastClient.JoinMulticastGroup(multicastEndpoint.Address);
            }
            catch (SocketException e)
            {
                Console.Error.Write(e.StackTrace);
            }
        }

        public void Run()
        {
            multicastClient.BeginReceive(ReadNext, null);

            while (true)
            {
                SendNext();
            }
        }

        public void ReadNext(IAsyncResult result)
        {
            Task.Run(() =>
            {
                IPEndPoint sender = new IPEndPoint(IPAddress.Any, 0);

                try
                {
                    byte[] message = multicastClient.EndReceive(result, ref sender);

                    Console.WriteLine($"{DateTime.Now}: |{Encoding.ASCII.GetString(message)}|");

                    multicastClient.BeginReceive(ReadNext, null);
                }
                catch (SocketException e)
                {
                    Console.Error.WriteLine(e.StackTrace);
                    Console.Error.WriteLine($"Eror: {e.ErrorCode}");
                }
            });
        }

        private void SendNext()
        {
            var nextMessage = Console.ReadLine();

            multicastClient.Send(Encoding.ASCII.GetBytes(nextMessage), nextMessage.Length, multicastEndpoint);
        }

        public void Dispose()
        {
            multicastClient.Dispose();
        }

        public static void Main()
        {
            using var echoMulticast = new MulticastEchoServer("225.0.0.0", 7777);

            echoMulticast.Run();
        }
    }
}
