using System;
using System.Collections.Generic;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace ilrd.WS06
{
    public abstract class ChatServer
    {
        protected readonly ClientManagerFactory clientManagerFactory;
        protected readonly ILogicManager logicManager;

        protected ChatServer(ClientManagerFactory clientManagerFactory, ILogicManager logicManager)
        {
            if(null == clientManagerFactory || null == logicManager)
            {
                throw new ArgumentException("Null can't be an argument");
            }

            this.clientManagerFactory = clientManagerFactory;
            this.logicManager = logicManager;
        }

        public abstract void Start();
        public abstract void Stop();
    }

    public class TCPChatServer : ChatServer
    {
        protected readonly TcpListener serverListener;
        private bool isRunning = true;

        public TCPChatServer(ClientManagerFactory clientManagerFactory, ILogicManager logicManager, IPEndPoint endpoint) : base(clientManagerFactory, logicManager)
        {
            serverListener = new TcpListener(endpoint);
        }

        public override void Start()
        {
            // This exception should be handled outside the method 
            serverListener.Start();

            while (isRunning)
            {
                try
                {
                    Socket newClientSocket = serverListener.AcceptSocket();
                    ClientManager newClientManager = clientManagerFactory.Invoke(newClientSocket);

                    newClientManager.OnMessage += logicManager.HandleMessage;
                    newClientManager.OnDisconnect += logicManager.HandleDisconnect;
                    newClientManager.OnProtocolError += logicManager.HandleProtocolError;

                    Console.WriteLine($"[{DateTime.Now}] New user added {newClientSocket.RemoteEndPoint}");

                    logicManager.HandleNewUser(newClientManager);
                    newClientManager.ReceiveNextAsync();
                }
                catch (SocketException e)
                {
                    Console.Error.WriteLine(e.StackTrace);
                }
            }
        }

        public override void Stop()
        {
            serverListener.Stop();
            isRunning = false;
        }
    }
}
