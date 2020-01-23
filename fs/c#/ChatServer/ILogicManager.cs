using System;
using System.Collections.Generic;
using System.Text;

namespace ilrd.WS06
{
    public interface ILogicManager
    {
        public void HandleMessage(ClientManager sender, string message);
        public void HandleDisconnect(ClientManager client);
        public void HandleNewUser(ClientManager newClient);
        public void HandleProtocolError(ClientManager client);
    }

    public sealed class TCPLogicManager : ILogicManager
    {
        private readonly Dictionary<ClientManager, UserInfo> users = new Dictionary<ClientManager, UserInfo>();

        public void HandleDisconnect(ClientManager client)
        {
            var leaveMessage = "has left the chat.";

            HandleMessage(client, leaveMessage);

            _ = users.Remove(client);
            client.Dispose();
        }

        public void HandleMessage(ClientManager sender, string message)
        {
            StringBuilder messageWithName = new StringBuilder();

            _ = users.TryGetValue(sender, out UserInfo senderInfo);

            if (senderInfo.UserName.Equals("Anon") && !message.Equals("has left the chat."))
            {
                senderInfo.UserName = message;
                message = "has joined the chat";
            }

            messageWithName.Append(senderInfo.UserName);
            messageWithName.Append(": ");
            messageWithName.Append(message);

            foreach (UserInfo user in users.Values)
            {
                user.Manager.Send(messageWithName.ToString());
            }
        }

        public void HandleNewUser(ClientManager newClient)
        {
            var newUserMessage = "Welcome! Please enter your username";

            newClient.Send(newUserMessage);
            users.Add(newClient, new UserInfo(newClient));
        }

        public void HandleProtocolError(ClientManager client)
        {
            client.DisconnectUser();
            HandleDisconnect(client);
        }
    }
}

