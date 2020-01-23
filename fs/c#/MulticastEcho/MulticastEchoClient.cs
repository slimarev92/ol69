using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace ilrd.WS06
{
    class MulticastEchoClient : IDisposable
    {
        private readonly UdpClient multicastClient;
        private readonly IPEndPoint multicastEndpoint;

        public MulticastEchoClient(string ip, int port)
        {
            IPAddress multicastIp = Dns.GetHostAddresses(ip)[0];
            multicastEndpoint = new IPEndPoint(multicastIp, port);

            multicastClient = new UdpClient();

            //This is done so other UDP clients on this machine can bind their socket to the multicast port.
            multicastClient.Client.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, 1);
            multicastClient.Client.Bind(new IPEndPoint(IPAddress.Any, port));

            multicastClient.JoinMulticastGroup(multicastEndpoint.Address);
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
            try
            {
                Console.Write("Enter port number:");
                var port = Int32.Parse(Console.ReadLine());

                using var echoMulticast = new MulticastEchoClient("225.0.0.0", port);
                echoMulticast?.Run();
            }
            catch (SocketException e)
            {
                Console.Error.WriteLine(e.StackTrace);
                Console.Error.WriteLine($"Eror: {e.ErrorCode}");
            }
        }
    }
}
