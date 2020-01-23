using System;
using System.Collections.Generic;
using System.Text;

namespace ilrd.WS06
{
    class UserInfo
    {
        private readonly ClientManager _manager;
        private string _userName = null;

        public string UserName { get => _userName ?? "Anon"; set => _userName = value; }
        public ClientManager Manager { get => _manager; }

        public UserInfo(ClientManager clientManager)
        {
            _manager = clientManager;
        }
    }
}
