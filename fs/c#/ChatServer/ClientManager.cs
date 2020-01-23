using System;
using System.Buffers;
using System.IO;
using System.Net.Sockets;
using System.Text;

namespace ilrd.WS06
{
    public delegate ClientManager ClientManagerFactory(Socket clientSocket);

    public abstract class ClientManager : IDisposable
    {
        protected readonly Socket clientSocket;
        protected bool disposed = false;

        public event Action<ClientManager, string> OnMessage;
        public event Action<ClientManager> OnDisconnect;
        public event Action<ClientManager> OnProtocolError;

        protected ClientManager(Socket clientSocket) => this.clientSocket = clientSocket;

        protected void RaiseNewMessageEvent(string message) => OnMessage?.Invoke(this, message);
        protected void RaiseDisconnectEvent() => OnDisconnect?.Invoke(this);
        protected void RaiseProtocolErrorEvent() => OnProtocolError?.Invoke(this);

        public abstract void ReceiveNextAsync();
        public abstract void Send(string message);
        public abstract void DisconnectUser();

        //TODO READ MORE ABOUT THIS
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (disposing && !disposed)
            {
                clientSocket.Dispose();
            }

            disposed = true;
        }
    }

    public class TCPClientManager : ClientManager
    {
        private const int BufferSize = 256;
        private const int MessageSizeBufferSize = 6;
        private const int BookendBytes = 2;
        private const byte MessageBookend = 0x1D;

        private static readonly ArrayPool<byte> bufferPool = ArrayPool<byte>.Shared;

        private readonly byte[] buffer;
        private int currMessageLen = 0;
        private MessageReceptionStatus currMessageReceptionStatus;
        private int currMessageRemainingBytes = 0;
        private MemoryStream currMessage = null;
        private int outgoingMessageLength = 0;

        public TCPClientManager(Socket clientSocket) : base(clientSocket) => buffer = bufferPool.Rent(BufferSize);

        public sealed override void ReceiveNextAsync()
        {
            currMessage?.Dispose();
            currMessage = new MemoryStream();

            clientSocket.BeginReceive(buffer, 0, MessageSizeBufferSize, SocketFlags.None, ReceiveLengthCallback, this);
        }

        private void SendCallback(IAsyncResult result)
        {
            TCPClientManager clientManager = (TCPClientManager)result.AsyncState;
            Socket clientSocket = clientManager.clientSocket;

            try
            {
                int sentBytes = clientSocket.EndSend(result);

                if (sentBytes < outgoingMessageLength)
                {
                    Console.Error.WriteLine($"[{DateTime.Now}] Error: sent bytes - {sentBytes}, message len - {outgoingMessageLength}");
                }
            }
            catch (SocketException e)
            {
                Console.Error.WriteLine(e.StackTrace);
            }
        }

        private void ReceiveMessageBodyCallback(IAsyncResult result)
        {
            TCPClientManager clientManager = (TCPClientManager)result.AsyncState;
            Socket clientSocket = clientManager.clientSocket;

            try
            {
                int bytesReceived = clientSocket.EndReceive(result);

                Console.WriteLine($"[{DateTime.Now}] Read {bytesReceived} bytes");

                if (0 == bytesReceived)
                {
                    Console.WriteLine($"[{DateTime.Now}] User left, disconnecting {clientSocket.RemoteEndPoint}");

                    RaiseDisconnectEvent();
                }
                else
                {
                    currMessage.Write(buffer, 0, bytesReceived);

                    if (0 < currMessageRemainingBytes)
                    {
                        var bytesToReceive = (currMessageRemainingBytes > BufferSize) ? BufferSize : currMessageRemainingBytes;
                        currMessageRemainingBytes -= bytesToReceive;

                        Console.WriteLine($"[{DateTime.Now}] Remaining bytes {currMessageRemainingBytes}");

                        clientSocket.BeginReceive(buffer, 0, bytesToReceive, SocketFlags.None, ReceiveMessageBodyCallback, this);
                    }
                    else
                    {
                        ValidateCurrentMessageBody();

                        switch (currMessageReceptionStatus)
                        {
                            case MessageReceptionStatus.Okay:
                                Console.WriteLine($"[{DateTime.Now}] Received valid message from {clientSocket.RemoteEndPoint}");
                                RaiseNewMessageEvent(GetMessageWithoutProtocol());

                                break;

                            case MessageReceptionStatus.ProtocolError:
                                Console.WriteLine($"[{DateTime.Now}] Protocol error with client {clientSocket.RemoteEndPoint}");
                                RaiseProtocolErrorEvent();

                                break;
                        }

                        ReceiveNextAsync();
                    }
                }
            }
            catch (SocketException e)
            {
                Console.Error.WriteLine(e.StackTrace);
                Console.WriteLine($"[{DateTime.Now}] Socket error, removing client {clientSocket.RemoteEndPoint}");

                RaiseDisconnectEvent();
            }
        }

        private string GetMessageWithoutProtocol()
        {
            var messageWithoutProtocol = new byte[currMessageLen - BookendBytes];

            currMessage.Seek(1, SeekOrigin.Begin);
            currMessage.Read(messageWithoutProtocol, 0, currMessageLen - BookendBytes);

            return Encoding.ASCII.GetString(messageWithoutProtocol);
        }

        private void ValidateCurrentMessageBody()
        {
            currMessage.Seek(0, SeekOrigin.Begin);

            var firstBookend = (byte)currMessage.ReadByte();

            currMessage.Seek(currMessageLen + BookendBytes, SeekOrigin.Begin);

            var secondBookend = (byte)currMessage.ReadByte();

            currMessageReceptionStatus = (MessageBookend == firstBookend) && (MessageBookend != secondBookend) ?
                MessageReceptionStatus.Okay :
                MessageReceptionStatus.ProtocolError;
        }

        private void DrainSocketCallback(IAsyncResult result)
        {
            //TODO FILL THIS
        }

        private void ReceiveLengthCallback(IAsyncResult result)
        {
            try
            {
                TCPClientManager clientManager = (TCPClientManager)result.AsyncState;
                Socket clientSocket = clientManager.clientSocket;

                int bytesReceived = clientSocket.EndReceive(result);

                SetMessageLen(bytesReceived);

                switch (currMessageReceptionStatus)
                {
                    case MessageReceptionStatus.Disconnect:
                        Console.WriteLine($"[{DateTime.Now}] Client left, removing client {clientSocket.RemoteEndPoint}");
                        RaiseDisconnectEvent();
                        break;

                    case MessageReceptionStatus.ProtocolError:
                        RaiseProtocolErrorEvent();
                        Console.WriteLine($"[{DateTime.Now}] Protocol error with client {clientSocket.RemoteEndPoint}");
                        break;

                    default:
                        var bytesToReceive = (currMessageRemainingBytes > BufferSize) ? BufferSize : currMessageRemainingBytes;
                        currMessageRemainingBytes -= bytesToReceive;

                        Console.WriteLine($"[{DateTime.Now}] Starting to read message ({currMessageLen} bytes)");

                        clientSocket.BeginReceive(buffer, 0, bytesToReceive, SocketFlags.None, ReceiveMessageBodyCallback, this);
                        break;
                }
            }
            catch (SocketException e)
            {
                Console.Error.WriteLine(e.StackTrace);
                Console.WriteLine($"[{DateTime.Now}] Socket error, removing client {clientSocket.RemoteEndPoint}");
                RaiseDisconnectEvent();
            }
        }

        private void SetMessageLen(int bytesReceived)
        {
            if (0 == bytesReceived)
            {
                currMessageReceptionStatus = MessageReceptionStatus.Disconnect;
            }
            else if (MessageBookend != buffer[0] || MessageBookend != buffer[bytesReceived - 1])
            {
                currMessageReceptionStatus = MessageReceptionStatus.ProtocolError;
            }
            else
            {
                currMessageLen = BitConverter.ToInt32(buffer, 1);
                currMessageReceptionStatus = MessageReceptionStatus.Okay;
            }

            currMessageRemainingBytes = currMessageLen;
        }

        public sealed override void Send(string message)
        {
            var messageWithProtocol = new byte[message.Length + BookendBytes];
            messageWithProtocol[0] = MessageBookend;
            messageWithProtocol[^1] = MessageBookend;
            Array.Copy(Encoding.ASCII.GetBytes(message), 0, messageWithProtocol, 1, message.Length);

            outgoingMessageLength = messageWithProtocol.Length;

            Console.WriteLine($"[{DateTime.Now}] Sending message ({outgoingMessageLength} bytes)");

            try
            {
                clientSocket.BeginSend(messageWithProtocol, 0, outgoingMessageLength, SocketFlags.None, SendCallback, this);
            }
            catch (SocketException e)
            {
                Console.Error.WriteLine(e.StackTrace);
            }
        }

        protected override void Dispose(bool disposing)
        {
            base.Dispose(disposing);
            bufferPool.Return(buffer);
        }

        public override void DisconnectUser() => clientSocket.Close();

        private enum MessageReceptionStatus
        {
            Okay,
            Disconnect,
            ProtocolError
        }
    }
}