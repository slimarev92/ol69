using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.ServiceModel.Web;
using System.Text;

// NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service" in code, svc and config file together.
namespace ilrd.WCF
{
    public class Item
    {
        public int id;
        public string item;
        public bool done;

        public Item(int id, string item, bool done)
        {
            this.id = id;
            this.item = item;
            this.done = done;
        }
    }

    public class TodoItemsService : ITodoItemsService
    {
        private readonly List<Item> userItems = new List<Item>();

        public List<Item> GetUserItems()
        {
            userItems.Add(new Item(1, "Wash the dishes", true));
            userItems.Add(new Item(1, "Walk the dog", false));
            userItems.Add(new Item(1, "Learn WCF", false));

            Console.WriteLine("GET REQUEST");

            return userItems;
        }
    }
}
